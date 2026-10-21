/*
 * A064992  a(n) = usigma(n+1) - usigma(n), where usigma(n) is the sum of
 *          unitary divisors of n (A034448).
 * https://oeis.org/A064992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = usigma(n+1) – usigma(n) where usigma(n) is the sum of
 *            unitary divisors of n (A034448)
 * Range:     i8 n <= 136, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   unitary_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/unitary_sigma.h"

seq_t a(idx_t n)
{
    /* Compute a(n) = usigma(n+1) - usigma(n) */
    seq_t sigma_n     = unitary_sigma(n);
    seq_t sigma_nplus = unitary_sigma(n + 1);
    return sigma_nplus - sigma_n;
}
