/*
 * A137340  a(n) = 2*a(n-1) + 3*a(n-2), with a(0) = 1, a(1) = 9.
 * https://oeis.org/A137340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=9, a(n)=2*a(n-1)+3*a(n-2) for n>=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: grows only, reuses previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries in cache */
    static size_t cache_cap = 0;   /* allocated capacity of cache */

    /* handle the base cases explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* first allocation of the cache with the two known values */
    if (!cache)
    {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache_len = 2;
    }

    /* if the requested index is not yet computed, extend the cache */
    size_t target = (size_t)n;
    if (target + 1 > cache_cap)
    {
        /* reallocate to accommodate up to target */
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
        /* compute new terms sequentially */
        for (size_t i = cache_len; i < new_cap; ++i)
        {
            cache[i] = cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)3;
        }
        cache_len = new_cap;
    }

    /* result is now guaranteed to be computed */
    return cache[(size_t)n];
}
