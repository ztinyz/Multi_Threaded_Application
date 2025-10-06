#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h> 

sem_t thread_started_sem[5];
sem_t thread_completed_sem[5];

sem_t thread_completed_sem_P2[5];
sem_t thread_started_sem_P2[5];
sem_t thread13_started;
sem_t thread13_completed;

sem_t thread_started_sem_P7[5];
sem_t thread_completed_sem_P7[5];

void *thread_function(void *arg) {
    void **args = (void **)arg;

    int thread_id = (int)(intptr_t)args[1];
    int process_id = (int)(intptr_t)args[0];
    sem_t **special_sem_vector = (sem_t **)args[2];
    sem_t *special_sem = special_sem_vector[0];
    sem_t *special_sem2 = special_sem_vector[1];

    int sleeping_number = 0;
    int value = 0;

    if(thread_id == 2){
        //printf("Thread 2 is waiting for thread 7.3 to end...\n");
        while(sem_getvalue(special_sem, &value) == 0)
        {
            if(value == 0)
            {
                for (int i = 0; i < 10000; i++) {
                    sleeping_number++;
                }
            }
            else
            {
                break;
            }
        }
        //printf("Thread 2 is allowed to start\n");
    }

    if(thread_id == 4) {
        // printf("Thread 4 is waiting for thread 3 to start...\n");
        // sem_getvalue(&thread_started_sem[3], &value);
        // printf("Sem 4 value: %d\n", value);
        sem_wait(&thread_started_sem[3]);
        //printf("Thread 4 start\n");
    }

    sem_post(&thread_started_sem[thread_id-1]);

    info(BEGIN, process_id, thread_id);

    // Simulate some work in the thread
    for (int i = 0; i < 10000; i++) {
        sleeping_number++;
    }

    // Thread 3 needs to wait for thread 4 to complete
    if(thread_id == 3) {
        sem_post(&thread_started_sem[3]); // Signal that thread 3 has started
        // printf("Thread 4 is allowed to start\n");
        // printf("Thread 3 is waiting for thread 4 to complete...\n");
        // sem_getvalue(&thread_completed_sem[2], &value);
        // printf("Sem 3 value: %d\n", value);
        sem_wait(&thread_completed_sem[2]); // Wait for thread 4 to signal completion
        // printf("complete...\n");
    }
    
    info(END, process_id, thread_id);

    sem_post(&thread_completed_sem[thread_id-1]);

    if(thread_id == 4) {
        sem_post(&thread_completed_sem[2]);
        // printf("thread 4 complete...\n");
    }

    if(thread_id == 2) {
        sem_post(special_sem2);
    }

    return NULL;
}

void *thread_function_P2(void *arg) {
    int *args = (int *)arg;
    int thread_id = args[1];
    int process_id = args[0];
    int sem_id = args[2];
    int sleeping_number = 0;
    int value1 = 0;
    int value2 = 0;
    int value3 = 0;
    int value4 = 0;
    // if(sem_id == 2)
    //     printf("TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG SAHUR!\n");
    if(thread_id == 13)
    {
        sem_post(&thread13_started);
    }
    else
        sem_post(&thread_started_sem_P2[sem_id - 1]);

    info(BEGIN, process_id, thread_id);


    // Simulate some work in the thread

    if(thread_id == 13){
        int flag = 1;
        while(flag == 1)
        {
            flag = 0;
            for(int i = 0; i < 5; i++){
                if(sem_getvalue(&thread_started_sem_P2[i], &value1) == 0){
                    //printf("Sem %d value: %d\n", i+1, value1);
                    sem_getvalue(&thread_completed_sem_P2[i], &value3);

                    //printf("Sem_start %d value: %d\n", i+1, value1);
                    //printf("Sem_finish %d value: %d\n", i+1, value3);
                    if(value1 != 1)
                    {
                        flag = 1;
                        break;
                    }
                    if(value3 != 0)
                    {
                        flag = 1;
                        break;
                    }
                }
                
            }
            if(flag == 0)
            {
                // for(int i = 0; i < 6; i++)
                // {
                //     sem_getvalue(&thread_started_sem_P2[i], &value1);
                //     printf("Sem %d value: %d\n", i+1, value1);
                // }
                info(END, process_id, thread_id);
                break;
            }
            for (int i = 0; i < 1000; i++) {
                sleeping_number++;
            }
        }
    }
    else
    {

        stai:
        for (int i = 0; i < 10000; i++) {
            sleeping_number++;
        }
        sem_getvalue(&thread13_completed, &value2);
        sem_getvalue(&thread13_started, &value4);
        if(value4 == 1 && value2 == 0)
        {
            goto stai;   
        }
        info(END, process_id, thread_id);
    }        

    if(thread_id == 13) {
        sem_post(&thread13_completed);
    }
    else
    {
        sem_post(&thread_completed_sem_P2[sem_id - 1]);
    }
    return NULL;
}

void *thread_function_P7(void *arg) {
    void **args = (void **)arg;

    int thread_id = (int)(intptr_t)args[1];
    int process_id = (int)(intptr_t)args[0];
    sem_t **special_sem_vector = (sem_t **)args[2];
    sem_t *special_sem = special_sem_vector[0];
    sem_t *special_sem2 = special_sem_vector[1];

    int sleeping_number = 0;
    int value = 0;

    if(thread_id == 1){
        //printf("Thread 1 is waiting for thread 5.2 to end...\n");
        while(sem_getvalue(special_sem2, &value) == 0)
        {
            if(value == 0)
            {
                for (int i = 0; i < 10000; i++) {
                    sleeping_number++;
                }
            }
            else
            {
                break;
            }
        }
        //printf("Thread 1 is allowed to start\n");
    }

    sem_post(&thread_started_sem_P7[thread_id-1]);

    info(BEGIN, process_id, thread_id);

    // Simulate some work in the thread
    for (int i = 0; i < 10000; i++) {
        sleeping_number++;
    }

    info(END, process_id, thread_id);


    // if(thread_id == 3) {
    //     printf("TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG SAHUR!\n");
    //     if(sem_getvalue(&thread_completed_sem_P7[2], &value) == 0)
    //         printf("Sem 3 value: %d\n", value);
    //     else
    //         printf("Error getting sem value\n");
    // }

    sem_post(&thread_completed_sem_P7[thread_id-1]);
    
    if(thread_id == 3) {
        sem_post(special_sem);
        // printf("TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG TUNG SAHUR!\n");
        // if(sem_getvalue(special_sem, &value) == 0)
        //     printf("Sem 2 value: %d\n", value);
        // else
        //     printf("Error getting sem value\n");
    }
    return NULL;
}

int create_process(int process_id, sem_t **special_sem) {

    int available_sem = 0;
    int sleeping_number = 0;
    int value = 0;

    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return -1;
    } else if (pid == 0) {

        info(BEGIN, process_id, 0);
        
        // Create children based on process_id
        if (process_id == 2) {
            create_process(3, special_sem);

            pthread_t thread_id[43];

            int args[43][3];

            for(int i = 0; i < 5; i++)
            {
                args[i][0] = process_id;
                args[i][1] = i + 1;
                args[i][2] = i + 1;

                if (pthread_create(&thread_id[i], NULL, thread_function_P2, (void *)args[i]) != 0) {
                    perror("Failed to create thread");
                    exit(EXIT_FAILURE);
                }
            }

            for(int i = 5; i < 43; i++)
            {
                nu_avem_semafor:
                available_sem = -1;
                if(i != 12)
                {
                    for(int j = 0; j < 5; j++)
                    {
                        if(sem_getvalue(&thread_completed_sem_P2[j], &value) == 0)
                        {
                            if(value == 1)
                            {
                                sem_destroy(&thread_started_sem_P2[j]);
                                sem_destroy(&thread_completed_sem_P2[j]);
                                sem_init(&thread_started_sem_P2[j], 0, 0);
                                sem_init(&thread_completed_sem_P2[j], 0, 0);
                                available_sem = j;
                                break;
                            }
                        }
                    }
                    if(available_sem == -1)
                    {
                        for(int j = 0; j < 1000; j++) {
                            sleeping_number++;
                        }
                        goto nu_avem_semafor;
                    }
                }
                args[i][0] = process_id;
                args[i][1] = i + 1;
                args[i][2] = available_sem + 1;

                if (pthread_create(&thread_id[i], NULL, thread_function_P2, (void *)args[i]) != 0) {
                    perror("Failed to create thread");
                    exit(EXIT_FAILURE);
                }
            }
            for(int i = 0; i < 43; i++)
            {
                if (pthread_join(thread_id[i], NULL) != 0) {
                    perror("Failed to join thread");
                    exit(EXIT_FAILURE);
                }
            }

        } 
        else if (process_id == 4) {
            create_process(5, special_sem);
            create_process(6, special_sem);
        }
        else if (process_id == 6) {
            create_process(7, special_sem);
        }
        else if (process_id == 5) {

            pthread_t thread_id[5];

            void** args[5];

            for(int i = 0; i < 5; i++)
            {
                args[i] = malloc(3 * sizeof(void*));
                args[i][0] = (void*)(intptr_t)process_id;
                args[i][1] = (void*)(intptr_t)(i + 1);
                args[i][2] = special_sem;

                if (pthread_create(&thread_id[i], NULL, thread_function, args[i]) != 0) {
                    perror("Failed to create thread");
                    exit(EXIT_FAILURE);
                }
            }
            for(int i = 0; i < 5; i++)
            {
                if (pthread_join(thread_id[i], NULL) != 0) {
                    perror("Failed to join thread");
                    exit(EXIT_FAILURE);
                }
            }

        }
        else if (process_id == 7)
        {

            pthread_t thread_id[5];

            void** args[5];
            for(int i = 0; i < 5; i++)
            {
                args[i] = malloc(3 * sizeof(void*));
                args[i][0] = (void*)(intptr_t)process_id;
                args[i][1] = (void*)(intptr_t)(i + 1);
                args[i][2] = special_sem;

                if (pthread_create(&thread_id[i], NULL, thread_function_P7, args[i]) != 0) {
                    perror("Failed to create thread");
                    exit(EXIT_FAILURE);
                }
            }
            for(int i = 0; i < 5; i++)
            {
                if (pthread_join(thread_id[i], NULL) != 0) {
                    perror("Failed to join thread");
                    exit(EXIT_FAILURE);
                }
            }
        }
        // Wait for all children to finish
        int status;
        while (wait(&status) > 0) {
            // Wait for child processes
        }

        info(END, process_id, 0);

        exit(0);
    }
    
    return pid; // Return pid to parent
}

int main(int argc, char **argv)
{
    // tester initialization
    // only one time in the main process
    init();

    info(BEGIN, 1, 0);

    for(int i = 0; i < 5; i++) {
        sem_init(&thread_started_sem_P7[i], 0, 0);
        sem_init(&thread_completed_sem_P7[i], 0, 0);

        sem_init(&thread_started_sem[i], 0, 0);
        sem_init(&thread_completed_sem[i], 0, 0);

        sem_init(&thread_started_sem_P2[i], 0, 0);
        sem_init(&thread_completed_sem_P2[i], 0, 0);
    }

    sem_init(&thread13_started, 0, 0);
    sem_init(&thread13_completed, 0, 0);

    sem_t *special_sem[2];
    special_sem[0] = sem_open("/my_special_sem_0", O_CREAT, 0644, 0);
    special_sem[1] = sem_open("/my_special_sem_1", O_CREAT, 0644, 0);
    if (special_sem[0] == SEM_FAILED || special_sem[1] == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }
    sem_init(special_sem[0], 0, 0); 
    sem_init(special_sem[1], 0, 0); 

    create_process(2, special_sem);
    create_process(4, special_sem);
    create_process(8, special_sem);

    int status;
    while (wait(&status) > 0) {
        // Wait for child processes
    }

    info(END, 1, 0);

    // Clean up semaphores
    for (int i = 0; i < 5; i++) {
        sem_destroy(&thread_started_sem_P7[i]);
        sem_destroy(&thread_completed_sem_P7[i]);

        sem_destroy(&thread_started_sem[i]);
        sem_destroy(&thread_completed_sem[i]);

        sem_destroy(&thread_started_sem_P2[i]);
        sem_destroy(&thread_completed_sem_P2[i]);
    }

    sem_close(special_sem[0]);
    sem_close(special_sem[1]);
    sem_unlink("/my_special_sem_0");
    sem_unlink("/my_special_sem_1");

    

}