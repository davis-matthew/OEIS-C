/*
 * A007610  Sum of n consecutive primes starting at a(n) is prime (or 0 if
 *          impossible).
 * https://oeis.org/A007610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Find the first prime p such that the sum of n consecutive primes
 *            starting at p is prime. Return 0 if no such p exists (only for
 *            even n)
 * Range:     u8 n <= 2544, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"


static uint64_t *primes = NULL;
static idx_t prime_count = 0;

static void generate_primes_sieve(uint64_t limit) {
    if (primes != NULL) return;
    
    bool *is_prime_arr = (bool *)malloc((limit + 1) * sizeof(bool));
    memset(is_prime_arr, true, (limit + 1) * sizeof(bool));
    is_prime_arr[0] = is_prime_arr[1] = false;
    
    for (uint64_t i = 2; i * i <= limit; i++) {
        if (is_prime_arr[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime_arr[j] = false;
            }
        }
    }
    
    prime_count = 0;
    for (uint64_t i = 0; i <= limit; i++) {
        if (is_prime_arr[i]) prime_count++;
    }
    
    primes = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    idx_t idx = 0;
    for (uint64_t i = 0; i <= limit; i++) {
        if (is_prime_arr[i]) {
            primes[idx++] = i;
        }
    }
    
    free(is_prime_arr);
}

seq_t a(idx_t n) {
    if (primes == NULL) {
        generate_primes_sieve(2000000);
    }
    
    if (prime_count < n) {
        return 0;
    }
    
    if (n % 2 == 0) {
        uint64_t sum = 0;
        for (idx_t i = 0; i < n; i++) {
            sum += primes[i];
        }
        
        if (is_prime(sum)) {
            return 2;
        } else {
            return 0;
        }
    }
    
    uint64_t sum = 0;
    for (idx_t j = 0; j < n; j++) {
        sum += primes[j];
    }
    
    if (is_prime(sum)) {
        return (seq_t)primes[0];
    }
    
    for (idx_t i = 1; i + n <= prime_count; i++) {
        sum = sum - primes[i - 1] + primes[i + n - 1];
        if (is_prime(sum)) {
            return (seq_t)primes[i];
        }
    }
    
    return 0;
}
