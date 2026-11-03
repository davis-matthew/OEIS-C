/*
 * A007445  Inverse Moebius transform of primes.
 * https://oeis.org/A007445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} prime(d)
 * Range:     u8 n <= 35, u16 n <= 2519, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 73 terms through n = 4983 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *sieve_and_extract_primes(uint64_t limit, idx_t *count) {
    bool *is_prime = (bool *)malloc(limit + 1);
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = false;
    
    for (uint64_t i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    *count = 0;
    for (uint64_t i = 2; i <= limit; i++) {
        if (is_prime[i]) (*count)++;
    }
    
    seq_t *primes = mem_new(seq_t, (*count * sizeof(seq_t)) / sizeof(seq_t));
    idx_t idx = 0;
    for (uint64_t i = 2; i <= limit; i++) {
        if (is_prime[i]) {
            primes[idx++] = (seq_t)i;
        }
    }
    
    free(is_prime);
    return primes;
}

seq_t a(idx_t n) {
    static seq_t *primes = NULL;
    static idx_t prime_count = 0;
    
    if (primes == NULL) {
        primes = sieve_and_extract_primes(1000000, &prime_count);
    }
    
    seq_t result = 0;
    
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            if (d <= prime_count) {
                result += primes[d - 1];
            }
            
            idx_t other_d = n / d;
            if (d != other_d && other_d <= prime_count) {
                result += primes[other_d - 1];
            }
        }
    }
    
    return result;
}
