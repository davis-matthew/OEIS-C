/*
 * A108204  a(n) = 2*(n-1)*a(n-1) -(n-1)*a(n-2) with a(0)=0, a(1)=1.
 * https://oeis.org/A108204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)*a(n-1) - (n-1)*a(n-2) with a(0)=0, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_len   = 0; /* number of already computed terms (next free index) */
    static idx_t cache_cap   = 0; /* allocated capacity */

    /* initialise cache for the first two terms */
    if (cache == NULL) {
        cache_cap = 4;                         /* start with a few entries */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure: return 0 */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* grow the cache if n is outside the current capacity */
    if ((idx_t)n >= cache_cap) {
        /* increase capacity until it can hold index n */
        while (cache_cap <= (idx_t)n)
            cache_cap <<= 1;                   /* double until large enough */

        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure: return 0 */
        cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i >= 2) {
            seq_t mult  = (seq_t)(i - 1);
            seq_t term1 = cache[i - 1];
            seq_t term2 = cache[i - 2];
            seq_t temp  = (seq_t)2 * term1;   /* 2 * a(n-1)  */
            seq_t diff  = temp - term2;       /* 2*a(n-1) - a(n-2) */
            cache[i] = mult * diff;           /* (n-1)*(2*a(n-1)-a(n-2)) */
        } else /* i==0 or i==1 are already initialised */ {
            /* nothing to do */
        }
    }
    /* update the length of the cache */
    cache_len = n + 1;
    return cache[n];
}
