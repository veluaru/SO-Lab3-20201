/**
 * @defgroup   SAXPY saxpy
 *
 * @brief      This file implements an iterative saxpy operation
 * 
 * @param[in] <-p> {vector size} 
 * @param[in] <-s> {seed}
 * @param[in] <-n> {number of threads to create} 
 * @param[in] <-i> {maximum itertions} 
 *
 * @author     Danny Munera
 * @date       2020
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

// Variables to obtain command line parameters
unsigned int seed = 1;
int p = 10000000;
int n_threads = 1;
int max_iters = 1000;
// Variables to perform SAXPY operation
double* X;
double a;
double* Y;
double* Y_avgs;
int i, it;

// Variables to get execution time
struct timeval t_start, t_end;
double exec_time;

//Objeto para definir limites de los hilos
typedef struct _param{
	int ini;
	int end;
} param_t;

//Semaforo
sem_t mutex;

void *funcionSaxpy(void *arg){
	int primero, segundo;
	double total;
	param_t* limites = (param_t *) arg;
	int ini = limites->ini;
	int end = limites->end;

	for(primero = 0; primero < max_iters; primero++){
		total = 0;
		for(segundo = ini; segundo < end; segundo++){
			Y[segundo] = Y[segundo] + a * X[segundo];
			total += Y[segundo];
		}
		sem_wait(&mutex);
		Y_avgs[primero] += total / p;	
		sem_post(&mutex);
	}
	return NULL;
};

void *unHilo(void *arg)
{
	pthread_t hilo1;
	param_t param1;
	param1.ini = 0;
	param1.end = p/1;
	pthread_create(&hilo1, NULL, funcionSaxpy, &param1);
	pthread_join(hilo1, NULL);
	return NULL;
};
void *dosHilos(void *arg)
{
	pthread_t hilo1;
	pthread_t hilo2;
	param_t param1;
	param1.ini = 0;
	param1.end = p/2;
	param_t param2;
	param2.ini = p/2;
	param2.end = p;
	pthread_create(&hilo1, NULL, funcionSaxpy, &param1);
	pthread_create(&hilo2, NULL, funcionSaxpy, &param2);
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);
	return NULL;
};
void *cuatroHilos(void *arg)
{
	pthread_t hilo1;
	pthread_t hilo2;
	pthread_t hilo3;
	pthread_t hilo4;
	param_t param1;
	param1.ini = 0;
	param1.end = p/4;
	param_t param2;
	param2.ini = p/4;
	param2.end = p/2;
	param_t param3;
	param3.ini = p/2;
	param3.end = p*3/4;
	param_t param4;
	param4.ini = p*3/4;
	param4.end = p;
	pthread_create(&hilo1, NULL, funcionSaxpy, &param1);
	pthread_create(&hilo2, NULL, funcionSaxpy, &param2);
	pthread_create(&hilo3, NULL, funcionSaxpy, &param3);
	pthread_create(&hilo4, NULL, funcionSaxpy, &param4);
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);
	pthread_join(hilo3, NULL);
	pthread_join(hilo4, NULL);
	return NULL;
};
void *ochoHilos(void *arg)
{
	pthread_t hilo1;
	pthread_t hilo2;
	pthread_t hilo3;
	pthread_t hilo4;
	pthread_t hilo5;
	pthread_t hilo6;
	pthread_t hilo7;
	pthread_t hilo8;
	param_t param1;
	param1.ini = 0;
	param1.end = p/8;
	param_t param2;
	param2.ini = p/8;
	param2.end = p/4;
	param_t param3;
	param3.ini = p/4;
	param3.end = p*3/8;
	param_t param4;
	param4.ini = p*3/8;
	param4.end = p/2;
	param_t param5;
	param5.ini = p/2;
	param5.end = p*5/8;
	param_t param6;
	param6.ini = p*5/8;
	param6.end = p*6/8;
	param_t param7;
	param7.ini = p*6/8;
	param7.end = p*7/8;
	param_t param8;
	param8.ini = p*7/8;
	param8.end = p;
	pthread_create(&hilo1, NULL, funcionSaxpy, &param1);
	pthread_create(&hilo2, NULL, funcionSaxpy, &param2);
	pthread_create(&hilo3, NULL, funcionSaxpy, &param3);
	pthread_create(&hilo4, NULL, funcionSaxpy, &param4);
	pthread_create(&hilo5, NULL, funcionSaxpy, &param5);
	pthread_create(&hilo6, NULL, funcionSaxpy, &param6);
	pthread_create(&hilo7, NULL, funcionSaxpy, &param7);
	pthread_create(&hilo8, NULL, funcionSaxpy, &param8);
	pthread_join(hilo1, NULL);
	pthread_join(hilo2, NULL);
	pthread_join(hilo3, NULL);
	pthread_join(hilo4, NULL);
	pthread_join(hilo5, NULL);
	pthread_join(hilo6, NULL);
	pthread_join(hilo7, NULL);
	pthread_join(hilo8, NULL);
	return NULL;
};

int main(int argc, char* argv[]){
	 if(argc!=1){
		char* palabra = argv[1];
	 	n_threads = palabra[0]-48;
	 }else{
	 	n_threads = 0;
	 }
	

	// Getting input values
	int opt;
	while((opt = getopt(argc, argv, ":p:s:n:i:")) != -1){  
		switch(opt){  
			case 'p':  
			printf("vector size: %s\n", optarg);
			p = strtol(optarg, NULL, 10);
			assert(p > 0 && p <= 2147483647);
			break;  
			case 's':  
			printf("seed: %s\n", optarg);
			seed = strtol(optarg, NULL, 10);
			break;
			case 'n':  
			printf("threads number: %s\n", optarg);
			n_threads = strtol(optarg, NULL, 10);
			break;  
			case 'i':  
			printf("max. iterations: %s\n", optarg);
			max_iters = strtol(optarg, NULL, 10);
			break;  
			case ':':  
			printf("option -%c needs a value\n", optopt);  
			break;  
			case '?':  
			fprintf(stderr, "Usage: %s [-p <vector size>] [-s <seed>] [-n <threads number>]\n", argv[0]);
			exit(EXIT_FAILURE);
		}  
	}  
	srand(seed);

	printf("p = %d, seed = %d, n_threads = %d, max_iters = %d\n", \
	 p, seed, n_threads, max_iters);	

	// initializing data
	X = (double*) malloc(sizeof(double) * p);
	Y = (double*) malloc(sizeof(double) * p);
	Y_avgs = (double*) malloc(sizeof(double) * max_iters);

	for(i = 0; i < p; i++){
		X[i] = (double)rand() / RAND_MAX;
		Y[i] = (double)rand() / RAND_MAX;
	}
	for(i = 0; i < max_iters; i++){
		Y_avgs[i] = 0.0;
	}
	a = (double)rand() / RAND_MAX;

#ifdef DEBUG
	printf("vector X= [ ");
	for(i = 0; i < p-1; i++){
		printf("%f, ",X[i]);
	}
	printf("%f ]\n",X[p-1]);

	printf("vector Y= [ ");
	for(i = 0; i < p-1; i++){
		printf("%f, ", Y[i]);
	}
	printf("%f ]\n", Y[p-1]);

	printf("a= %f \n", a);	
#endif

	/*
	 *	Function to parallelize 
	 */
	gettimeofday(&t_start, NULL);
	sem_init(&mutex, 0, 1);
	//SAXPY iterative SAXPY mfunction
	if(argc==1){
		unHilo(NULL);
	}else{

		if((strcmp(argv[1], "1")==0)){
			unHilo(NULL);
		}
		if((strcmp(argv[1], "2")==0)){
			dosHilos(NULL);
		}
		if((strcmp(argv[1], "4")==0)){
			cuatroHilos(NULL);
		}
		if((strcmp(argv[1], "8")==0)){
			ochoHilos(NULL);
		}
	}
	gettimeofday(&t_end, NULL);

#ifdef DEBUG
	printf("RES: final vector Y= [ ");
	for(i = 0; i < p-1; i++){
		printf("%f, ", Y[i]);
	}
	printf("%f ]\n", Y[p-1]);
#endif
	
	// Computing execution time
	exec_time = (t_end.tv_sec - t_start.tv_sec) * 1000.0;  // sec to ms
	exec_time += (t_end.tv_usec - t_start.tv_usec) / 1000.0; // us to ms
	printf("Execution time: %f ms \n", exec_time);
	printf("Last 3 values of Y: %f, %f, %f \n", Y[p-3], Y[p-2], Y[p-1]);
	printf("Last 3 values of Y_avgs: %f, %f, %f \n", Y_avgs[max_iters-3], Y_avgs[max_iters-2], Y_avgs[max_iters-1]);
	return 0;
}	