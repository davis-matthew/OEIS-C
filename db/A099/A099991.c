/*
 * A099991  a(n) = Moebius(2n).
 * https://oeis.org/A099991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = μ(2n) (Möbius function evaluated at 2·n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n) {
    /* Compute 2*n as an idx_t to avoid overflow, then use the
       helper function defined in utils/mobius.h. */
    idx_t two_n = n << 1;          /* 2*n */
    return mobius(two_n);
}
