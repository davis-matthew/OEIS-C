/*
 * A086831  Ramanujan sum c_n(2).
 * https://oeis.org/A086831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) = μ(n) if n is odd * a(n) = μ(n) + 2·μ(n/2) if n is even *
 *            where μ denotes the Möbius function
 * Range:     i8 n <= 9625, i16 n <= 9625, i32 n <= 9625, i64 n <= 9625,
 *            i128 n <= 9625, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* μ(n) always fits in a signed integer (-1, 0, or 1). */
    seq_t mu_n = mobius(n);
    if ((n & 1) == 1)            /* n is odd */
        return mu_n;
    /* n is even */
    seq_t mu_half = mobius(n >> 1);
    return mu_n + (seq_t)2 * mu_half;
}
