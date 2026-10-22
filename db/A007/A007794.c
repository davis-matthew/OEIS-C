/*
 * A007794  Juxtapose pairs of primes (starting at 1).
 * https://oeis.org/A007794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenate the (2n-1)th and (2n)th elements of [1,
 *            primes]
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 2245, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"


static uint64_t *prime_cache = NULL;
static idx_t cache_capacity = 0;
static idx_t prime_cache_len = 0;
static uint64_t next_candidate = 2;

static void extend_prime_cache(idx_t needed_primes) {
    if (prime_cache_len >= needed_primes) return;
    
    if (cache_capacity < needed_primes) {
        idx_t new_capacity = (cache_capacity == 0) ? 100 : cache_capacity;
        while (new_capacity < needed_primes) {
            new_capacity *= 2;
        }
        
        uint64_t *new_cache = (uint64_t *)malloc(new_capacity * sizeof(uint64_t));
        if (prime_cache != NULL) {
            for (idx_t i = 0; i < prime_cache_len; i++) {
                new_cache[i] = prime_cache[i];
            }
            free(prime_cache);
        }
        prime_cache = new_cache;
        cache_capacity = new_capacity;
    }
    
    while (prime_cache_len < needed_primes) {
        if (is_prime(next_candidate)) {
            prime_cache[prime_cache_len] = next_candidate;
            prime_cache_len++;
        }
        next_candidate++;
    }
}

static seq_t get_element(idx_t pos) {
    if (pos == 1) return (seq_t)1;
    
    idx_t prime_index = pos - 1;
    extend_prime_cache(prime_index);
    
    return (seq_t)prime_cache[prime_index - 1];
}

seq_t a(idx_t n) {
    seq_t num1 = get_element(2*n - 1);
    seq_t num2 = get_element(2*n);
    
    seq_t temp = num2;
    int digits = 0;
    if (temp == 0) {
        digits = 1;
    } else {
        while (temp > 0) {
            digits++;
            temp /= 10;
        }
    }
    
    seq_t power = 1;
    for (int i = 0; i < digits; i++) {
        power *= 10;
    }
    
    return num1 * power + num2;
}
