/*
 * A077967  Expansion of 1/(1+2*x^2+x^3).
 * https://oeis.org/A077967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1, a(1) = 0, a(2) = -2, a(n) = -2*a(n-2) - a(n-3) for n ≥
 *            3
 * Range:     i8 n <= 12, i16 n <= 28, i32 n <= 54, i64 n <= 110,
 *            i128 n <= 223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterative DP with lazy caching */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) abort();

        /* Copy existing values */
        if (cache) {
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }

        /* Compute missing values */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0)
                new_cache[i] = (seq_t)1;
            else if (i == 1)
                new_cache[i] = (seq_t)0;
            else if (i == 2)
                new_cache[i] = (seq_t)(-2);
            else {
                seq_t val = new_cache[i-2] * ((seq_t)2) + new_cache[i-3];
                new_cache[i] = -val;
            }
        }

        cache        = new_cache;
        cache_size   = new_size;
    }

    return cache[(size_t)n];
}
