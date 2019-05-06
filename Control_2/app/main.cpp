/* 
 * File:   main.cpp
 * Author: michel
 *
 * Created on 5 de mayo de 2019, 21:15
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime> 
#include <mpi.h> 

int main(int argc, char** argv) {
    int mi_rango; /* rango del proceso    */
    int p; /* numero de procesos   */
    int fuente; /* rango del que envia  */
    int dest; /* rango del que recibe */
    int tag = 0; /* etiqueta del mensaje */
    char mensaje[100]; /* mensaje  */
    char nom_maquina[30]; //nombre de la maquina
    int len_nom=0; //largo del nombre
    
    MPI_Status estado; /* devuelve estado al recibir*/

    /* Comienza las llamadas a MPI */
    MPI_Init(&argc, &argv);

    /* Obtener el largo y nombre de la máquina */
    MPI_Get_processor_name(nom_maquina, &len_nom);

    /* Averiguamos el rango de nuestro proceso */
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango);

    
    time_t tiempo;
    tiempo=time(0); // se almacena el valor actual de tiempo del sistema 
    struct tm t= *localtime(&tiempo);

    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (mi_rango != 0) {
        sprintf(mensaje,"Nombre maquina: %s \t Fecha: %d-%d-%d %d:%d:%d", 
                nom_maquina, t.tm_year+1900, t.tm_mon+1, t.tm_mday, 
                t.tm_hour, t.tm_min, t.tm_sec); //imprime el nombre de la maquina y fecha actual en formato yyyy-mm-dd
        
        
        dest = 0;
        /* Usa strlen+1 para que la marca /0 se transmita */
        MPI_Send(mensaje, strlen(mensaje) + 1, MPI_CHAR,
                dest, tag, MPI_COMM_WORLD);
        
    } 
    
    else 
    { 
        for (fuente = 1; fuente < p; fuente++) {
            MPI_Recv(mensaje, 100, MPI_CHAR, fuente,
                    tag, MPI_COMM_WORLD, &estado);
            printf("%s\n", mensaje);
        }
    }

    
    MPI_Finalize();
    return EXIT_SUCCESS;
}
