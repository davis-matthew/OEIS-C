/*
 * A024123  a(n) = 10^n - n^9.
 * https://oeis.org/A024123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 10^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 9, i64 n <= 18, i128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 10^n */
    seq_t ten_pow = pow10(n);

    /* Compute n^9 */
    seq_t base = (seq_t)n;
    seq_t nine_pow = 1;
    for (idx_t i = 0; i < 9; ++i)
        nine_pow *= base;

    return ten_pow - nine_pow;
}
