/*
 * A078004  Expansion of (1-x)/(1-2*x+2*x^2+x^3).
 * https://oeis.org/A078004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) - a(n-3), with a(0) = 1, a(1) = 1,
 *            a(2) = 0
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 42, i64 n <= 85, i128 n <= 169,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    idx_t i;

    /* Initialise cache with first three terms if needed. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;  /* out of memory; undefined behaviour in test harness */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 0;
    }

    /* Return already computed value. */
    if (n < cache_len)
        return cache[n];

    /* Expand cache up to index n. */
    for (i = cache_len; i <= n; ++i) {
        /* Compute recurrence using seq_t arithmetic. */
        seq_t val = 2 * cache[i-1] - 2 * cache[i-2] - cache[i-3];
        if (i == cache_len) {
            size_t new_len = (size_t)(i + 1);
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) return 0;  /* out of memory */
            cache = tmp;
            cache_len = new_len;
        }
        cache[i] = val;
    }

    return cache[n];
}
