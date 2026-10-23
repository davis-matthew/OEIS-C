/*
 * A153275  Numbers n such that 10*n+1 is not prime.
 * https://oeis.org/A153275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer m (m ≥ 0) with 10·m + 1 composite
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset 1: a(1) = 0 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Handle non‑positive indices.  The OEIS offset is 1, so we simply return 0 for n < 1. */
    if (n < 1)
        return 0;

    /* Static cache for memoisation.  The cache holds the values of a(1), a(2), … */
    static seq_t *cache = NULL;   /* pointer to dynamically allocated array of seq_t */
    static size_t cache_sz = 0;   /* number of cached values */
    static size_t cache_cap = 0;  /* current capacity of the cache */
    static idx_t next_candidate = 0;   /* next k to test for the property 10*k+1 composite */

    /* Initialise the cache on the first call. */
    if (cache_cap == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Grow the cache until it contains at least n elements. */
    while (cache_sz < (size_t)n) {
        /* Determine if 10*next_candidate + 1 is composite.  1 is treated as composite. */
        idx_t test_num = 10ULL * next_candidate + 1ULL;
        if (!is_prime_small(test_num)) {
            /* Store the candidate in the cache. */
            if (cache_sz == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz] = (seq_t)next_candidate;
            cache_sz++;
        }
        next_candidate++;
    }

    return cache[n - 1];
}
