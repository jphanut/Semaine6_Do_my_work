#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
* Function used to launch threads that will do the work for us.
*
* @long_computing : a function that will do a really long computing and that must be started by the thread.
*                 This function initially returns a pointer to an int and takes as argument a string.
*                 These have been casted into void* to match pthread_create() function specifications.
*                 You have to launch this function with the argument *string* and compare the returned value
*                 to *ret_value*. If the return value doesn't match *ret_value*,
*                 you have to launch another thread on the same argument with the function *backup_computing*.
*
* @backup_computing :  a backup function that must be used if the first thread didn't do the job correctly.
*                    As for *long_computing*, this function initially takes a string as argument and returns
*                    a pointer to an int.
*
* @string : the string that must be used as a parameter to both functions described above.
*
* @ret_value : the value to which the first returned code must be compared to to know if the first thread
*             correctly did the job or not.
*
* @return the int returned by the function started by the thread if no problem
*         -1 if you can't create the first thread
*         -2 if you can't join the first thread
*         -3 if you can't create the second thread
*         -4 if you can't join the second thread
*/
int rc = 1000;

int * func1 (char * string) {
    printf("%s", string);
    int * result = malloc(sizeof(int));
    * result = 1000;
    return (void *)result;
}

int * func2(char * string) {
    printf("%s", string);
    int * result = malloc(sizeof(int));
    * result = 1000;
    return (void *)result;
}

int doMyWork(void* long_computing(void*), void* backup_computing(void*), char* string, int ret_value){
    pthread_t thread1, thread2;
    int rc, rc_thread = 0;
    int * ptr_rc_thread = NULL;
    if ( 0 != (rc=pthread_create(&thread1, NULL, long_computing, string))) return -1;
    if ( 0 != (rc=pthread_join(thread1, (void **)&ptr_rc_thread))) return -2;
    rc_thread = * ptr_rc_thread;
    if ( ret_value == rc_thread ) return rc_thread;
    if ( 0 != (rc=pthread_create(&thread2, NULL, backup_computing, string))) return -3;
    if ( 0 != (rc=pthread_join(thread2, (void **)&ptr_rc_thread))) return -4;
    rc_thread = * ptr_rc_thread;
    return rc_thread;
}

int main()
{
    char * string ="0123456789";
    int ret_value = 1000;
    int rc =  doMyWork((void *)func1, (void *)func2, string, ret_value);
    printf("RC %d\n", rc);
    return 0;
}
