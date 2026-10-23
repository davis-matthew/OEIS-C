/*
 * A159506  Numerator of Hermite(n, 12/13).
 * https://oeis.org/A159506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A159506: a(n) = 13^n * Hermite(n, 12/13)
 * Recurrence:  A_0 = 1,  A_1 = 24
 *              A_{n+1} = 24*A_n - 338*n*A_{n-1}
 *  (holds for all n >= 0; i.e. A_2 = 24*A_1 - 338*1*A_0, etc.)
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;       /* number of computed entries   */
    static const seq_t const24 = (seq_t)24;
    static const seq_t const338 = (seq_t)338;

    /* initialise cache for the first request */
    if (cache_len == 0) {
        cache_len = (n >= 1 ? n + 1 : 2);          /* at least two elements: A0, A1   */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)24;
        for (idx_t i = 2; i <= n; ++i) {
            seq_t mulfac = const338 * (seq_t)(i - 1);
            cache[i] = const24 * cache[i - 1] - mulfac * cache[i - 2];
        }
        cache_len = n + 1;
        return cache[n];
    }

    /* extend cache if needed */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* allocation failed; return 0 as safe default */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t mulfac = const338 * (seq_t)(i - 1);
            cache[i] = const24 * cache[i - 1] - mulfac * cache[i - 2];
        }
        cache_len = new_len;
    }

    return cache[n];
}
