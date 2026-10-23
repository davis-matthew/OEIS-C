/*
 * A159646  Numerator of Hermite(n, 8/19).
 * https://oeis.org/A159646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 19^n * Hermite(n, 8/19) Recurrence derived: a(0) = 1,
 *            a(1) = 16 a(n+1) = 16 * a(n) - 722 * n * a(n-1) for n >= 1 The
 *            coefficients are exact integers, no floating‑point operations
 *            used
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached computed values: cache[i] == a(i) for i < cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize cache for n==0 and n==1 */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)16;  /* a(1) */
    }

    /* Compute up to requested n */
    if (n >= cache_len) {
        /* Expand cache to hold index n */
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = 16*a(i-1) - 722*(i-1)*a(i-2) */
            const seq_t term1 = cache[i - 1] * (seq_t)16;
            const seq_t factor = (seq_t)(722 * (i - 1));
            const seq_t term2 = cache[i - 2] * factor;
            cache[i] = term1 - term2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
