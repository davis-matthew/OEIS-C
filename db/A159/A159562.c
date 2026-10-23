/*
 * A159562  Numerator of Hermite(n, 13/18).
 * https://oeis.org/A159562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 9^n * Hermite(n,13/18). Recurrence: a(0) = 1, a(1) = 13,
 *            a(n) = 13*a(n-1) - 162*(n-1)*a(n-2) (for n ≥ 2)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 13, i128 n <= 24,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* memory holding computed terms */
    static size_t cache_cap  = 0;      /* capacity of cache in terms of seq_t elements */
    static idx_t  cache_len  = 0;      /* number of terms stored (next free index) */

    /* Ensure the cache is large enough to hold term n */
    if (n >= cache_cap) {
        ensure_cache(n, &cache, &cache_cap);   /* ensure_cache guarantees capacity > n */
    }

    /* Compute any missing terms up to n */
    if (cache_len <= n) {
        /* initialise the first two terms if needed */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        if (cache_len == 1 && n >= 1) {
            cache[1] = (seq_t)13;
            cache_len = 2;
        }

        /* Use the recurrence to fill the cache */
        for (idx_t i = cache_len; i <= n; ++i) {
            idx_t  prev      = i - 1;
            idx_t  prevprev  = i - 2;
            seq_t  term_i_minus_1 = cache[prev];
            seq_t  term_i_minus_2 = cache[prevprev];
            seq_t  mult = (seq_t)162 * (seq_t)(prev) * term_i_minus_2;
            cache[i] = (seq_t)13 * term_i_minus_1 - mult;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
