/*
 * A160107  Numerator of Hermite(n, 7/27).
 * https://oeis.org/A160107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 7/27) * Recurrence: a(0) = 1, a(1) =
 *            14, a(n) = 14*a(n-1) - 1458*(n-1)*a(n-2) for n ≥ 2 * * The
 *            function is memoised so that calls in any order are handled
 *            efficiently
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;    /* pointer to stored terms */
    static size_t cache_len = 0;   /* number of terms currently cached */

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; caller may choose to handle this */
            return 0;
        }
        cache = tmp;

        /* initialise new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)14;
            } else {
                /* a(i) = 14*a(i-1) - 1458*(i-1)*a(i-2) */
                seq_t term1 = cache[i - 1] * (seq_t)14;
                seq_t factor = (seq_t)(1458 * (idx_t)(i - 1));
                seq_t term2 = cache[i - 2] * factor;
                cache[i] = term1 - term2;
            }
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
