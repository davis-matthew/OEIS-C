/*
 * A160004  Numerator of Hermite(n, 2/25).
 * https://oeis.org/A160004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1 * a(1) = 4 * a(n) = 4*a(n-1) - 1250*(n-1)*a(n-2) (n ≥
 *            2) * This recurrence follows from the Hermite polynomial
 *            identity * H_{n+1}(x) = 2x H_n(x) - 2n H_{n-1}(x) with x = 2/25
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored */

    /* Grow the cache to include term n if necessary. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) abort();                    /* allocation failed */

        /* Compute missing terms incrementally. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                tmp[i] = (seq_t)1;
            else if (i == 1)
                tmp[i] = (seq_t)4;
            else {
                seq_t term1 = (seq_t)4 * tmp[i-1];
                seq_t term2 = (seq_t)1250 * (seq_t)(i-1) * tmp[i-2];
                tmp[i] = term1 - term2;
            }
        }

        cache = tmp;
        cache_len = new_len;
    }

    return cache[n];
}
