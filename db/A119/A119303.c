/*
 * A119303  Expansion of (1 - 3x)/(1 - x + 2x^2 - x^3).
 * https://oeis.org/A119303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) - 2*a(n-2) + a(n-3); a(0) = 1, a(1) = -2, a(2) =
 *            -4
 * Range:     i8 n <= 14, i16 n <= 34, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t used = 0;                /* number of terms currently stored */

    /* ensure storage for indices up to n */
    if (used <= n) {
        idx_t new_len = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
        } else {
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        }
        /* initial terms if not yet set */
        if (used == 0) {
            cache[0] = (seq_t)1;
            used = 1;
        }
        if (used == 1 && n >= 1) {
            cache[1] = (seq_t)-2;
            used = 2;
        }
        if (used == 2 && n >= 2) {
            cache[2] = (seq_t)-4;
            used = 3;
        }
        /* compute remaining terms using recurrence */
        for (idx_t i = used; i <= n; ++i) {
            cache[i] = cache[i-1] - (seq_t)2 * cache[i-2] + cache[i-3];
        }
        used = new_len;
    }
    return cache[n];
}
