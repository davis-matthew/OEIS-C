/*
 * A024022  a(n) = 2^n - n^12.
 * https://oeis.org/A024022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 38, i128 n <= 127,
 *            big any n
 * Helpers:   pow2, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t two_pow = pow2(n);
    seq_t n_pow  = pow_int((seq_t)n, 12);
    return two_pow - n_pow;
}
