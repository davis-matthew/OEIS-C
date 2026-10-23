/*
 * A126794  a(1)=1. a(n+1) = a(n) + a(floor(sqrt(n))).
 * https://oeis.org/A126794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126794: a(1) = 1, a(n+1) = a(n) + a(floor(sqrt(n))) */

#include <stdlib.h>

/* Integer square‑root using binary search (no floating point) */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] unused, cache[1] = 1          */
    static idx_t  cache_len = 0;     /* largest index currently stored         */
    static size_t cache_cap = 0;     /* allocated capacity (elements)          */

    /* Allocate or grow cache to hold up to index n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            abort();                  /* out‑of‑memory – stop execution       */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Lazily initialise the cache */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;          /* unused, but keeps indices 1‑based     */
        cache[1] = (seq_t)1;          /* a(1) = 1                                */
        cache_len = 1;
    }

    /* Compute missing terms up to n on demand */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t prev = cache[i];            /* a(i)                                 */
        idx_t k = floor_isqrt_u64(i);     /* floor(sqrt(i))                       */
        seq_t add = cache[k];             /* a(floor(sqrt(i)))                    */
        cache[i + 1] = prev + add;        /* a(i+1) = a(i) + a(floor(sqrt(i)))   */
    }
    cache_len = n;

    return cache[n];
}
