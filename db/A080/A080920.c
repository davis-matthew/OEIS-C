/*
 * A080920  a(n) = 2*a(n-1) + 35*a(n-2), a(0) = 0, a(1) = 1.
 * https://oeis.org/A080920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 35*a(n-2); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms   */
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;   /* number of valid entries in cache */
    static size_t  cache_cap = 0;   /* allocated capacity of cache     */

    /* initialize cache on first call                                       */
    if (cache_cap == 0) {
        cache_cap = 8;                           /* start with small capacity   */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;             /* allocation failure fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* if requested index already in cache, return it */
    if ((idx_t)cache_len > n) {
        return cache[(size_t)n];
    }

    /* grow cache capacity if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed) new_cap <<= 1;  /* double until fit */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure fallback */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing entries up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        cache[(size_t)k] = (seq_t)2 * cache[(size_t)(k-1)] + (seq_t)35 * cache[(size_t)(k-2)];
    }
    cache_len = needed;        /* update length to full size needed */

    return cache[(size_t)n];
}
