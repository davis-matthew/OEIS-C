/*
 * A181435  First column in matrix inverse of a mixed convolution of A052906.
 * https://oeis.org/A181435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(n) for n != 2; a(2) = -4 (special case)
 * Range:     i8 n <= 1091, i16 n <= 1091, i32 n <= 1091, i64 n <= 1091,
 *            i128 n <= 1091, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)2)
        return (seq_t)-4;
    return mobius(n);
}
