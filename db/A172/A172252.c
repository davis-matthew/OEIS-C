/*
 * A172252  a(n) = 4*2^n - 9.
 * https://oeis.org/A172252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*2^n - 9
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);   /* compute 2^n */
    v = v * 4;           /* multiply by 4 */
    v = v - 9;           /* subtract 9 */
    return v;
}
