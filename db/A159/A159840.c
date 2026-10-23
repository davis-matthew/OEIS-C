/*
 * A159840  Numerator of Hermite(n, 15/22).
 * https://oeis.org/A159840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 15*a(n-1) - 242*(n-1)*a(n-2) Initial values: a(0) = 1,
 *            a(1) = 15 Recurrence derived from OEIS: a(n) -15*a(n-1)
 *            +242*(n-1)*a(n-2)=0
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0; /* number of elements currently in cache */

    /* Ensure space for a(0) and a(1) */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0; /* fallback, unlikely to happen */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
    }

    /* Return already computed value */
    if ((idx_t)cache_len > n) return cache[n];

    /* Grow cache to hold indices up to n */
    size_t old_len = cache_len;
    size_t new_len = (size_t)n + 1;
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!tmp) return 0; /* fallback */
        for (size_t i = 0; i < old_len; ++i) tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cache_len = new_len;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t term1 = (seq_t)15 * cache[i - 1];
        seq_t term2 = ((seq_t)(i - 1)) * cache[i - 2];
        term2 *= (seq_t)242;
        cache[i] = term1 - term2;
    }

    return cache[n];
}
