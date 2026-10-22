/*
 * A004648  a(n) = prime(n) mod n.
 * https://oeis.org/A004648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) mod n
 * Range:     u8 n <= 344, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


static bool *sieve = NULL;
static idx_t sieve_size = 0;
static uint64_t *primes = NULL;
static idx_t prime_count = 0;

static void ensure_sieve(idx_t limit) {
    if (sieve_size >= limit) return;
    
    sieve = (bool *)realloc(sieve, (limit + 1) * sizeof(bool));
    memset(sieve, 1, (limit + 1) * sizeof(bool));
    sieve[0] = sieve[1] = 0;
    
    for (idx_t i = 2; i * i <= limit; i++) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                sieve[j] = 0;
            }
        }
    }
    sieve_size = limit;
}

static void ensure_primes(idx_t n) {
    if (prime_count >= n) return;
    
    idx_t limit = 200000;
    ensure_sieve(limit);
    
    if (primes == NULL) {
        primes = (uint64_t *)malloc(n * sizeof(uint64_t));
    } else {
        primes = (uint64_t *)realloc(primes, n * sizeof(uint64_t));
    }
    
    idx_t count = 0;
    for (idx_t i = 2; i <= sieve_size && count < n; i++) {
        if (sieve[i]) {
            primes[count++] = i;
        }
    }
    prime_count = count;
}

seq_t a(idx_t n) {
    ensure_primes(n);
    return (seq_t)(primes[n - 1] % n);
}
