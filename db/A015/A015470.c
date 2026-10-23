/*
 * A015470  q-Fibonacci numbers for q=12, scaling a(n-2).
 * https://oeis.org/A015470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 12^(n-2) * a(n-2) with initial values a(0) = 0,
 *            a(1) = 1 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the n-th term of OEIS sequence A015470. */
seq_t a(idx_t n)
{
    /* static cache for incremental computation */
    static seq_t *cache = NULL;     /* array of computed values */
    static idx_t cache_size = 0;    /* number of stored elements */

    /* power of 12 currently needed for the next computation step:
       pow = 12^(i-2) when i equals the index we are about to compute.
       Initially pow = 1 for i = 2. */
    static seq_t pow = 1;

    /* If the requested index is already cached, return it. */
    if (cache && n < cache_size)
        return cache[n];

    /* Ensure we have at least two entries for the base cases. */
    if (cache_size == 0) {
        /* Allocate space for two base terms */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) { /* out of memory – abort gracefully */
            cache[0] = 0; /* unlikely to be evaluated */
            return 0;
        }
        cache[0] = 0;  /* a(0) */
        cache[1] = 1;  /* a(1) */
        cache_size = 2;
        pow = 1; /* for computing a(2) */
    }

    /* If the requested index is still less than the cached size, return. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to hold at least n+1 elements. */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* realloc failed – keep existing cache and return last computed value */
        return cache[cache_size - 1];
    }
    cache = new_cache;

    /* Compute the sequence from the current cache_size up to n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* a(i) = a(i-1) + 12^(i-2) * a(i-2) */
        seq_t term = cache[i - 1] + pow * cache[i - 2];
        cache[i] = term;
        /* Next power: 12^(i-1-2) = 12^(i-2) * 12 */
        pow = pow * (seq_t)12;
    }

    /* Update cache size. */
    cache_size = n + 1;

    return cache[n];
}
