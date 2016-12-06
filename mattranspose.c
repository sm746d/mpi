//Program to calculate matrix transpose using inter-loop MPI communication
#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<string.h>

int main(void)
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  const char *s = " ";
  char *token = NULL;

  int i = 0,j = 0, m, n,r,p;
  int a[200][200],b[200][200]={0} ;

  MPI_Init(NULL, NULL);

  // Get the number of processes
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  // Get the rank of the process
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // Get the name of the processor
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_len;
  MPI_Get_processor_name(processor_name, &name_len);

  if(rank==0)
  {
  //    printf("Matrix file read from processor %s, rank %d out of %d processors\n", processor_name, rank, size);
      fp = fopen("data2.txt", "r");
      if (fp == NULL)
      {
        printf("Error opening");
        exit(EXIT_FAILURE);
      }
    
      while ((read=getline(&line, &len, fp)) != -1)
      {
         token = strtok(line, s);
         while(token != NULL)
         {
           a[i][j] = atoi(token);
   //        printf("a[%d][%d] %d\n", i, j, arr[i][j]);
           token=strtok(NULL,s);
           j++;
         }
         n=j;     //N is number of columns
         i++;
         j=0;
      }
      m=i;        //M is num of rows
      MPI_Send(&m,1,MPI_INT,1,0,MPI_COMM_WORLD);
      MPI_Send(&n,1,MPI_INT,1,0,MPI_COMM_WORLD);

      //matrixTranMaster(arr[i][j],m,n);
      for(i=0;i<m;i++)
      {
        for(j=0;j<n;j++)
        {
        printf(" %d \t", a[i][j]);  
        MPI_Send(&a[i][j],1,MPI_INT,1,0,MPI_COMM_WORLD);
        }     
            printf("\n");
      }
  }
    
  else 
  {
      MPI_Recv(&r,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      MPI_Recv(&p,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      printf("Number  of Rows::%d\n",r);
      printf("Number  of Columns::%d\n",p);
      for(i=0;i<r;i++)
      { 
        for(j=0;j<p;j++)
        {
          MPI_Recv(&b[j][i],1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        }
      }   
    
      for(i=0;i<p;i++)
      {
        for(j=0;j<r;j++)
        {
          printf("%d\t",b[i][j]);
        }
      printf("\n");
      } 
  }

  MPI_Finalize();
  exit(EXIT_SUCCESS);
  return 0;
}
