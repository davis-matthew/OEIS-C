/*
 * A138230  Expansion of (1-x)/(1 - 2*x + 4*x^2).
 * https://oeis.org/A138230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 4*a(n-2), a(0) = 1, a(1) = 1
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)1;   /* a(1) */
    if (n == 0) return a0;
    if (n == 1) return a1;

    seq_t ai;
    for (idx_t i = 2; i <= n; ++i) {
        ai = a1 * (seq_t)2 - a0 * (seq_t)4;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
