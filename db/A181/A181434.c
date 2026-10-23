/*
 * A181434  First column in matrix inverse of a mixed convolution of A052542.
 * https://oeis.org/A181434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1; a(2)=-3; a(n)=μ(n) for n>=3 (the Möbius function; see
 *            utils/mobius.h)
 * Range:     i8 n <= 1085, i16 n <= 1085, i32 n <= 1085, i64 n <= 1085,
 *            i128 n <= 1085, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n) {
    if (n == 2) return (seq_t)-3;
    return mobius(n);
}
