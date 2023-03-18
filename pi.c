# include <stdio.h>
  # include <stdlib.h>
  # include <math.h>
  # include "mpi.h"
  #include <time.h>

int main(int argc, char * argv[]) {

  int myid, numprocs;

  double PI25DT = 3.141592653589793238462643;
  double t1, t2;

  long long npts = 1e10;
  long long i, mynpts;

  long double f, sum, mysum;
  long double xmin, xmax, x;

  // Initialization routine => starts the MPI environment
  // Defines the communicator MPI_COMM_WORLD
  MPI_Init( & argc, & argv);
  // Determines the number of processes in a communicator
  MPI_Comm_size(MPI_COMM_WORLD, & numprocs);
  // Determines the rank (id) of the calling process in the communicator
  MPI_Comm_rank(MPI_COMM_WORLD, & myid);

  // Data decomposition . Each process gets a part of the work
  mynpts = npts / numprocs;

  if (myid == 0) {
    // Returns the time in seconds since an arbitrary time in the past
    t1 = MPI_Wtime();
  }

  mysum = 0.0;
  xmin = 0.0;
  xmax = 1.0;

  // Seed the pseudo random number generator
  srand(time(0));

  for (i = 0; i < mynpts; i++) {
    // ( long double ) rand () /( long double ) RAND_MAX
    // returns a random number between 0 and 1.
    // ( long double ) rand () /( long double ) RAND_MAX *( xmax - xmin )
    // returns a random number between 0 and max - min.
    // the whole expression will return a random number between 0+ min and min +( max - min )
    // = > between min and max .
    //x = ((long double) rand() / (long double) (RAND_MAX)) * (xmax - xmin) + xmin;
    x=(long double)(rand()) / ((long double)(RAND_MAX) + 1.0);
    // Each process does a partial sum over its own points .
    mysum += 4.0 / (1.0 + x * x);
  }

  // Take all the processes values of mysum and add them up into sum on process 0.
  MPI_Reduce( &mysum, &sum, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (myid == 0) {
    // Returns the time in seconds since an arbitrary time in the past
    t2 = MPI_Wtime();

    f = sum / npts;

    printf("Pi calculated with % lld points . \n ", npts);
    printf("Pi calculated : %.16Lf \n", f);
    printf("Correct value of Pi : %.16f \n", PI25DT);
    printf("Error is : %.16f \n", fabs(f - PI25DT));
    printf("Elapsed time [s] for the relevant part of the program : %f \n", t2 - t1);
  }

  // Stop the MPI environment
  MPI_Finalize();
}
