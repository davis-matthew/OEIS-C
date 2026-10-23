/*
 * A160391  Numerator of Hermite(n, 11/32).
 * https://oeis.org/A160391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of Hermite(n, 11/32) * Recurrence: a(0)=1,
 *            a(1)=11, a(n)=11*a(n-1)-512*(n-1)*a(n-2) for n≥2
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) return 0; /* memory failure, return 0 as fallback */
        cache = new_cache;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                cache[0] = (seq_t)1;
            else if (i == 1)
                cache[1] = (seq_t)11;
            else {
                seq_t mult = (seq_t)512 * (seq_t)(i - 1);
                cache[i] = (seq_t)11 * cache[i - 1] - mult * cache[i - 2];
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}
