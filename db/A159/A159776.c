/*
 * A159776  Numerator of Hermite(n, 17/21).
 * https://oeis.org/A159776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=34; for n>=2: a(n)=34*a(n-1)-882*(n-1)*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored terms (highest index + 1) */

    /* initialise cache on first call */
    if (cache_len == 0) {
        idx_t init_len = 2;
#if defined(__cplusplus)
        cache = new seq_t[init_len];
#else
        cache = (seq_t*)malloc(init_len * sizeof(seq_t));
#endif
        if (cache == NULL) return (seq_t)0;   /* allocation failure safety */
        cache[0] = (seq_t)1;        /* a(0) */
        cache[1] = (seq_t)34;       /* a(1) */
        cache_len = init_len;
    }

    /* If we need more entries, grow the cache */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + (idx_t)1;   /* we need indices 0..n inclusive */

#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[new_len];
        for (idx_t i = 0; i < old_len; ++i)
            new_cache[i] = cache[i];
        delete [] cache;
        cache = new_cache;
#else
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
#endif
        if (cache == NULL) return (seq_t)0; /* allocation failure safety */
        cache_len = new_len;

        /* compute the missing terms (from old_len to n) */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t term1 = (seq_t)34 * cache[i - (idx_t)1];
            seq_t term2 = (seq_t)882 * (seq_t)(i - (idx_t)1) * cache[i - (idx_t)2];
            cache[i] = term1 - term2;
        }
    }

    return cache[n];
}
