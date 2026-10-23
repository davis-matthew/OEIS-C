/*
 * A192423  Constant term of the reduction by x^2 -> x+2 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - a(n-3) - a(n-4) Initial values:
 *            a(0)=2, a(1)=0, a(2)=4, a(3)=2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static cache for already computed values */
static seq_t *cache = NULL;        /* allocated array of seq_t   */
static size_t cache_cap = 0;       /* current allocated capacity */
static size_t cache_len = 3;       /* index of the last computed value */

/* Ensure that values a[0..n] are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    /* Nothing to do if we have already computed up to n. */
    if ((size_t)n <= cache_len)
        return;

    /* Grow the cache if necessary. */
    size_t needed = (size_t)n + 1;                    /* we need indices 0..n inclusive */
    if (cache_cap < needed) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;                            /* grow geometrically */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL)                        /* OOM – silently abort */
            return;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialize the first four terms if the cache was empty. */
    if (cache_len == (size_t)3 && cache_cap > 0 && cache[0] == 0 && cache[1] == 0 && cache[2] == 0 && cache[3] == 0) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)2;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i-1]
                 + cache[i-2] * (seq_t)4
                 - cache[i-3]
                 - cache[i-4];
    }

    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    /* Handle the first four terms without using the cache. */
    if (n < 4) {
        static const seq_t init[4] = { (seq_t)2, (seq_t)0, (seq_t)4, (seq_t)2 };
        return init[(size_t)n];
    }

    ensure_cache(n);
    return cache[(size_t)n];
}
