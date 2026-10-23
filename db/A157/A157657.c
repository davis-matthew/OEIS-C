/*
 * A157657  a(1) = 1, a(n) = -mu(n) for n >= 2.
 * https://oeis.org/A157657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 for n = 1, otherwise a(n) = -mu(n)
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the provided mu_of utility. */
#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    int mu = mu_of(n);
    return (seq_t)(-mu);
}
