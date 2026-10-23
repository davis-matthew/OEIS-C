/*
 * A157658  a(1) = 0, a(n) = -mu(n) for n >= 2.
 * https://oeis.org/A157658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 0, a(n) = -mu(n) for n >= 2
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    return -(seq_t)mu_of(n);
}
