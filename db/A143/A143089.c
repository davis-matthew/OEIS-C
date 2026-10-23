/*
 * A143089  a(n) = a(n - a(n-1)) + a(floor(2*n/3)).
 * https://oeis.org/A143089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - a(n-1)) + a(floor(2*n/3)), with a(0)=1, a(1)=1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoization cache for sequence values */
static seq_t *cache = NULL;
static size_t  cache_cap = 0;    /* allocated capacity */
static idx_t   cache_len = 0;    /* number of elements currently stored */

/* Ensure cache has space for indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_len > n)
        return;

    idx_t needed = n + 1; /* indices 0..n inclusive */
    if (cache_cap < (size_t)needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap < (size_t)needed)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialize base cases, if not already set */
    if (cache_len == 0) {
        cache[0] = 1;          /* a(0) */
        cache_len = 1;
        if (needed > 1) {
            cache[1] = 1;      /* a(1) */
            cache_len = 2;
        }
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) { cache[0] = 1; continue; }
        if (i == 1) { cache[1] = 1; continue; }

        idx_t idx1 = i - (idx_t)cache[i - 1];          /* floor(2*i/3) later */
        idx_t idx2 = (2 * i) / 3;
        cache[i] = cache[idx1] + cache[idx2];
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
