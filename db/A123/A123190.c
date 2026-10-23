/*
 * A123190  Sum of the entries of the first row of the matrix M^n, where M is
 *          the 4 X 4 matrix [[ -1, 3, -3, 1 ], [ 3, -6, 3, 0 ], [ -3, 0, 3, 0
 *          ], [ 1, 4, 1, 0 ]].
 * https://oeis.org/A123190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) + 34*a(n-2) + 30*a(n-3) - 108*a(n-4) (a(0)=1,
 *            a(1)=0, a(2)=6, a(3)=-6)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 12, i64 n <= 23, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Initialise cache with first four terms on first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;                /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)(-6);
        cache_len = 4;
    }

    /* Return already computed term */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to at least n+1 terms */
    idx_t required = n + (idx_t)1;
    if (required > cache_cap) {
        size_t new_cap = (size_t)required * sizeof(seq_t);
        seq_t *new_cache = (seq_t *)malloc(new_cap);
        if (new_cache == NULL) return 0;                /* allocation failure */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache     = new_cache;
        cache_cap = required;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val = (seq_t)-4 * cache[i - 1]
                  + (seq_t)34 * cache[i - 2]
                  + (seq_t)30 * cache[i - 3]
                  - (seq_t)108 * cache[i - 4];
        cache[i] = val;
    }
    cache_len = required;

    return cache[n];
}
