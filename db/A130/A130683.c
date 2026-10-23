/*
 * A130683  Duplicate of A032184.
 * https://oeis.org/A130683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 4 * 2^n * (n+1)!
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)4;
    /* multiply by 2^n */
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)2;
    }
    /* multiply by (n+1)! */
    for (idx_t i = 2; i <= n + 1; ++i) {
        res *= (seq_t)i;
    }
    return res;
}
