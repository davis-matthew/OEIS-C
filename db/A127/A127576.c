/*
 * A127576  Primes of the form 16n+15.
 * https://oeis.org/A127576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime that is congruent to 15 modulo 16
 * Range:     u8 n <= 7, u16 n <= 647, u32 n <= 748, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of previously computed terms */
    static seq_t *cache          = NULL;   /* array of primes found so far */
    static size_t cache_cap      = 0;     /* capacity of the array */
    static size_t cache_size     = 0;     /* number of elements stored */
    static uint64_t next_cand    = 31;    /* next number of the form 16k+15 to test */
    static bool initialized      = false;

    /* Initialise on first call */
    if (!initialized) {
        cache_cap   = 64;                         /* arbitrary start size */
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        initialized = true;
    }

    /* Generate primes until we have at least n terms */
    while (cache_size < (size_t)n) {
        if (is_prime_big((seq_t)next_cand)) {
            /* Grow the cache if necessary */
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = (seq_t)next_cand;
        }
        next_cand += 16;   /* next candidate of the form 16k+15 */
    }

    return cache[n - 1];
}
