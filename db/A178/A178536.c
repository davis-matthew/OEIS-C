/*
 * A178536  First column of A178535.
 * https://oeis.org/A178536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k|n} mobius(n/k)*([k==1] - [2|k]) Equivalent to:
 *            a(n) = mu(n) for n != 2; a(2) = -2
 * Range:     i8 n <= 800, i16 n <= 800, i32 n <= 800, i64 n <= 800,
 *            i128 n <= 800, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)(-2);
    return mobius(n);
}
