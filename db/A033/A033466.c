/*
 * A033466  Denominators of the first differences of 1/(n^2 + 1).
 * https://oeis.org/A033466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (2*n+1)/((n²+1)*((n+1)²+1))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* cast index to seq_t */
    seq_t sn = (seq_t)n;

    /* compute n^2 + 1 and (n+1)^2 + 1 */
    seq_t n_sq   = sn * sn + 1;
    seq_t sn1    = sn + 1;
    seq_t n1_sq  = sn1 * sn1 + 1;

    /* denominator of the fraction before reduction */
    seq_t denom   = n_sq * n1_sq;

    /* numerator of the fraction */
    seq_t numer   = sn * 2 + 1;

    /* common divisor of numerator and denominator */
    seq_t g = gcd(denom, numer);

    /* return reduced denominator */
    return denom / g;
}
