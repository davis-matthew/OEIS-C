/*
 * A005145  n copies of n-th prime.
 * https://oeis.org/A005145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, tabl
 * Formula:   a(n) = prime(floor(1/2 + sqrt(2*n)))
 * Range:     u8 n <= 1485, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>


static bool sieve_initialized = false;
static seq_t *primes = NULL;
static idx_t prime_count = 0;

static void init_primes(void) {
    if (sieve_initialized) return;
    
    const int limit = 10000;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    
    for (int i = 0; i <= limit; i++) {
        is_prime[i] = true;
    }
    is_prime[0] = is_prime[1] = false;
    
    for (int i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    prime_count = 0;
    for (int i = 0; i <= limit; i++) {
        if (is_prime[i]) prime_count++;
    }
    
    primes = (seq_t *)malloc(prime_count * sizeof(seq_t));
    idx_t idx = 0;
    for (int i = 0; i <= limit; i++) {
        if (is_prime[i]) {
            primes[idx++] = (seq_t)i;
        }
    }
    
    free(is_prime);
    sieve_initialized = true;
}

seq_t a(idx_t n) {
    init_primes();
    
    idx_t k = (idx_t)(0.5 + sqrt(2.0 * (double)n));
    
    return primes[k - 1];
}
