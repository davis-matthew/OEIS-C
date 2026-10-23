/*
 * A133695  a(n) = 2*A008683 - 1.
 * https://oeis.org/A133695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*μ(n) - 1, where μ(n) is the Mobius function (OEIS
 *            A008683)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* Compute the Mobius function for n and apply the formula */
    seq_t m = mobius(n);   /* μ(n), will be -1, 0, or 1 */
    return 2 * m - 1;
}
