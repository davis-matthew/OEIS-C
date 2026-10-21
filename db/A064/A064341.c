/*
 * A064341  Generalized Catalan numbers C(3,3; n).
 * https://oeis.org/A064341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(0) = 1, a(1) = 1
 * Recurrence for n ≥ 2:
 *   a(n) = ( 3*(23*n-60)*a(n-1) + 54*(2*n-3)*a(n-2) ) / (2*(n-1))
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to memoize previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Ensure the cache can hold values up to index n */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed; in a production setting you might abort
               or handle the error, but here we simply return 0. */
            return (seq_t)0;
        }
        cache = new_cache;
        for (size_t i = cache_len; i < new_len; ++i)
            cache[i] = 0;     /* Initialize new entries to 0 */
        cache_len = new_len;
    }

    /* Base cases */
    if (n == 0 || n == 1) {
        cache[n] = (seq_t)1;
        return cache[n];
    }

    /* Compute missing values up to n, using the recurrence */
    for (idx_t i = 2; (size_t)i <= n; ++i) {
        if (cache[i] != 0)
            continue; /* Already computed */

        idx_t i_minus_1 = i - 1;
        idx_t i_minus_2 = i - 2;

        /* Terms in the recurrence */
        seq_t term23 = (seq_t)(23 * (int)i - 60);
        seq_t term2  = (seq_t)(2 * (int)i - 3);

        seq_t numerator = (seq_t)3 * term23 * cache[i_minus_1]
                        + (seq_t)54 * term2  * cache[i_minus_2];

        seq_t denom = (seq_t)2 * (seq_t)i_minus_1; /* 2*(n-1) */

        cache[i] = numerator / denom; /* Integer division is exact */
    }

    return cache[n];
}
