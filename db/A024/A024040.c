/*
 * A024040  a(n) = 4^n - n^4.
 * https://oeis.org/A024040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^4
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t pow4n = pow4(n);
    seq_t nn = (seq_t)n;
    seq_t n4 = nn * nn * nn * nn;
    return pow4n - n4;
}
