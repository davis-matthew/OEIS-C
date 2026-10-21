/*
 * A099492  A Chebyshev transform of the Padovan-Jacobsthal numbers.
 * https://oeis.org/A099492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -2*a(n-2) + 2*a(n-3) - 2*a(n-4) - a(n-6) Initial values:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=-1, a(5)=-4 This recurrence
 *            is valid for all n ≥ 6
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to reuse previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries currently stored */

    /* initialise cache with the first 6 terms when first called */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure: return zero */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)(-1);
        cache[5] = (seq_t)(-4);
    }

    /* shrink if n is smaller than already stored maximum (no-op for cache_len>=n+1) */
    if (n < cache_len) {
        return cache[n];
    }

    /* grow the cache to accommodate up to n */
    if (n >= cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure: return zero */
        cache = tmp;
        /* compute the new entries */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence uses indices i-2, i-3, i-4, i-6 which are all valid
               because i ≥ cache_len (≥6) after initialization            */
            cache[i] = (-2) * cache[i - 2]
                     +  2 * cache[i - 3]
                     - 2 * cache[i - 4]
                     -   cache[i - 6];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
