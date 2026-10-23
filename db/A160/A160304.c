/*
 * A160304  Numerator of Hermite(n, 6/31).
 * https://oeis.org/A160304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 12, a(n+2) = 12*a(n+1) - 1922*(n+1)*a(n) (OEIS
 *            A160304)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache storing values that have already been computed. */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;   /* allocated size                      */
    static idx_t computed_len = 0;      /* number of values that are valid     */

    /* Grow the cache if needed to accommodate index n. */
    if (n >= cache_capacity) {
        idx_t new_cap = (cache_capacity == 0) ? 1 : cache_capacity * 2;
        while (new_cap <= n + 1) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failure */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = computed_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i == 1) {
            val = 12;
        } else {
            /* Recurrence: a(i) = 12*a(i-1) - 1922*(i-1)*a(i-2) */
            val = 12 * cache[i - 1] - (seq_t)1922 * (seq_t)(i - 1) * cache[i - 2];
        }
        cache[i] = val;
    }
    computed_len = (n >= computed_len) ? n + 1 : computed_len;

    return cache[n];
}
