/*
 * A003805  Order of universal Chevalley group A_7 (q), q = prime power.
 * https://oeis.org/A003805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = q^28 * Product_{i=2..8} (q^i - 1), where q = A000961(n+1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 3, big any n
 * Helpers:   a000248_pow, compare_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/a000248_pow.h"
#include "../utils/compare_uint64.h"

static bool *sieve_of_eratosthenes(uint64_t limit) {
    bool *is_prime = (bool *)malloc(limit + 1);
    memset(is_prime, true, limit + 1);
    is_prime[0] = is_prime[1] = false;
    
    for (uint64_t i = 2; i * i <= limit; i++) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) {
                is_prime[j] = false;
            }
        }
    }
    
    return is_prime;
}

static uint64_t *generate_prime_powers(uint64_t limit, idx_t *count) {
    bool *is_prime = sieve_of_eratosthenes(limit);
    
    uint64_t *powers = (uint64_t *)malloc(12000 * sizeof(uint64_t));
    idx_t p_count = 0;
    
    powers[p_count++] = 1;
    
    for (uint64_t p = 2; p <= limit; p++) {
        if (is_prime[p]) {
            uint64_t power = p;
            while (power <= limit) {
                powers[p_count++] = power;
                if (power > limit / p) break;
                power *= p;
            }
        }
    }
    
    qsort(&powers[1], (size_t)(p_count - 1), sizeof(uint64_t), compare_uint64);
    
    free(is_prime);
    *count = p_count;
    return powers;
}

seq_t a(idx_t n) {
    static uint64_t *prime_powers = NULL;
    static idx_t prime_powers_count = 0;
    
    if (prime_powers == NULL) {
        prime_powers = generate_prime_powers(100000, &prime_powers_count);
    }
    
    if (n >= (idx_t)prime_powers_count) {
        return 0;
    }
    
    uint64_t q_ll = prime_powers[n];
    seq_t q = (seq_t)q_ll;
    
    seq_t q_28 = a000248_pow(q, 28);
    
    seq_t product = 1;
    for (int i = 2; i <= 8; i++) {
        seq_t q_i = a000248_pow(q, (idx_t)i);
        product *= (q_i - 1);
    }
    
    return q_28 * product;
}
