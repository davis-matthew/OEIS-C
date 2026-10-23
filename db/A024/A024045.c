/*
 * A024045  a(n) = 4^n-n^9.
 * https://oeis.org/A024045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Helpers:   pow4, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t p4 = pow4(n);
    seq_t p9 = pow_int((seq_t)n, 9);
    return p4 - p9;
}
