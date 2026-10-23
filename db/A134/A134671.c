/*
 * A134671  Primes of the form 2m*691 - 1.
 * https://oeis.org/A134671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 14, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A134671: primes of the form 2*m*691 - 1
   a(1) = 1381 = 2*691 - 1
   a(n) is the nth prime of this form, for n >= 1
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Computes the nth term a(n) of the sequence */
seq_t a(idx_t n)
{
    if (n < 1) {
        /* Undefined for n == 0; return 0 as a safe default */
        return (seq_t)0;
    }

    /* Static state to memoise computed primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_m = 1;          /* next m to test */
    static idx_t primes_found = 0;    /* number of primes stored */

    /* Grow the cache when needed */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; return 0 as a fallback */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate primes of the required form until we have n of them */
    while (primes_found < n) {
        /* Compute 2*m*691 - 1 in seq_t arithmetic */
        seq_t p = ((seq_t)2 * (seq_t)691 * (seq_t)next_m) - (seq_t)1;
        if (is_prime_big(p)) {
            cache[cache_len++] = p;
            primes_found++;
            /* Ensure cache capacity for next insertion after loop */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) {
                    return (seq_t)0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
        }
        next_m++;
    }

    /* Return the requested term */
    return cache[(size_t)(n - 1)];
}
