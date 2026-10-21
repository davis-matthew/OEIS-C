/*
 * A080042  a(n) = 4*a(n-1)+3*a(n-2) for n>1, a(0)=2, a(1)=4.
 * https://oeis.org/A080042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 4, a(n) = 4*a(n‑1) + 3*a(n‑2) (n > 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow arbitrary call order */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_sz = 0;   /* number of valid entries in cache */

    /* initialise cache with first two terms on first call */
    if (cache_sz == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) abort();         /* out of memory */
        cache[0] = (seq_t)2;         /* a(0) */
        cache[1] = (seq_t)4;         /* a(1) */
        cache_sz = 2;
    }

    /* fast return if we already have the value */
    if ((idx_t)cache_sz > n) return cache[n];

    /* grow the cache to accommodate the requested index */
    size_t new_sz = (size_t)n + 1;
    if (new_sz > cache_sz) {
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        /* WARNING: cache_sz is still the old size; we need it to start the computation */
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = (idx_t)cache_sz; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)4 + cache[i-2] * (seq_t)3;
    }

    /* update the size of the cached array */
    cache_sz = new_sz;
    return cache[n];
}
