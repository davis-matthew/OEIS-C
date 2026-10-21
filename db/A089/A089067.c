/*
 * A089067  a(n) = 2*a(n-1) + (-1)^n*a(floor(n/2)); a(1)=1.
 * https://oeis.org/A089067
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(n)=2*a(n-1)+(-1)^n*a(floor(n/2)) for n≥2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 1‑based cache of previously computed terms. */
static seq_t *cache = NULL;      /* cache[0] unused, cache[1] = a(1)          */
static idx_t  cache_cap = 0;     /* size of allocated array (≥0)             */
static idx_t  cache_nmax = 0;    /* maximum n that has a(n) stored           */

/* Ensure that a(1)…a(n) are populated in the cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_nmax)
        return;        /* already available */

    /* Grow the cache array if necessary (doubling strategy). */
    if (cache_cap < n + 1) {
        idx_t new_cap = cache_cap == 0 ? 2 : cache_cap;
        while (new_cap < n + 1)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();   /* Allocation failed. */

        cache = tmp;

        /* Initialize the new elements to zero. */
        for (idx_t i = cache_cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;

        cache_cap = new_cap;
    }

    /* Seed the base case if this is the first call. */
    if (cache_nmax == 0) {
        cache[1] = (seq_t)1;
        cache_nmax = 1;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_nmax + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)2;
        idx_t half = i / 2;
        if ((i & 1) == 0)          /* even i: add a(i/2) */
            val = val + cache[half];
        else                       /* odd i: subtract a(floor(i/2)) */
            val = val - cache[half];
        cache[i] = val;
    }

    cache_nmax = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined region; return 0 */

    ensure_cache(n);
    return cache[n];
}
