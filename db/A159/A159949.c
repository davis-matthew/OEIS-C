/*
 * A159949  Numerator of Hermite(n, 1/24).
 * https://oeis.org/A159949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=1, a(n)=a(n-1)-288*(n-1)*a(n-2) (n>=2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of H_n(1/24) = 12^n * H_n(1/24) */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache has at least two elements for n=0,1 */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }

    /* If requested index already computed, return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Resize cache to accommodate up to index n */
    cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (cache == NULL) { /* out of memory */
        return 0;
    }

    /* Compute missing terms using recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = a(i-1) - 288*(i-1)*a(i-2) */
        seq_t term = cache[i - 1];
        seq_t mult = (seq_t)288 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term - mult;
    }
    cache_len = n + 1;
    return cache[n];
}
