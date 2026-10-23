/*
 * A165751  a(n) = 4 - 3*2^n.
 * https://oeis.org/A165751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4 - 3*2^n
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 61, i128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_n = pow2(n);          /* 2^n */
    seq_t val   = two_n * (seq_t)3; /* 3*2^n */
    return ((seq_t)4) - val;        /* 4 - 3*2^n */
}
