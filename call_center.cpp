//  Materials used:
//  https://lauraliparulo.altervista.org/java-threads-synchronization-with-semaphore/

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t operatorSemaphore;
const int numOperators = 3;

void *customerThread(void *arg) {
  const char *customerName = (const char *)arg;
  int semaphoreValue;
  std::cout << customerName << "  is waiting to speak to the operator...\n";
  sem_getvalue(&operatorSemaphore, &semaphoreValue);
  if (semaphoreValue == 0) {
    std::cout << "All operators are currently busy assisting other callers - "
                 "MUSIC ...\n";
  }
  sem_wait(&operatorSemaphore); // Wait for an available operator
  // Critical Section: Phone call handling
  std::cout << customerName
            << " is getting the connection to the operator...\n";
  // Simulate work during the call
  sleep(8);
  std::cout << customerName
            << " is now ending the conversation with the operator...\n";
  sem_post(&operatorSemaphore); // Release the operator
  sem_getvalue(&operatorSemaphore, &semaphoreValue);
  std::cout << "Available operators: " << semaphoreValue << std::endl;
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[5];
  const char *customerNames[] = {"Alice", "Bob", "John", "Mark", "Alex"};

  // Initialize semaphore with the number of available operators
  sem_init(&operatorSemaphore, 0, numOperators);

  std::cout
      << "Welcome to the call center. Please wait for the setup to start\n";
  std::cout << "Initial operator availability = " << numOperators << "\n";

  // Create customer threads
  for (int i = 0; i < 5; ++i) {
    if (pthread_create(&threads[i], NULL, customerThread,
                       (void *)customerNames[i]) != 0) {
      std::cerr << "Thread creation failed...\n";
      return 1;
    }

    // Simulate time between calls
    sleep(1);
  }

  // Wait for all threads to finish
  for (int i = 0; i < 5; ++i) {
    pthread_join(threads[i], NULL);
  }

  sem_destroy(&operatorSemaphore);

  return 0;
}
