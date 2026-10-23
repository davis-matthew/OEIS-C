/*
 * A162442  Denominators of the column sums of the EG1 matrix coefficients.
 * https://oeis.org/A162442
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = denom((1/(n-1))*(2*n-1)*binomial(2*n-2,n-1)/4^(n-1))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, gcd, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* below offset, value irrelevant */

    /* numerator: (2*n-1) * binomial(2*n-2, n-1) */
    seq_t num = seq_mul((seq_t)(2*n - 1), binomial(2*n - 2, n - 1));

    /* denominator: (n-1) * 4^(n-1)  = (n-1) * 2^(2*(n-1)) */
    seq_t denom = seq_mul((seq_t)(n - 1), pow2(2*(n - 1)));

    /* cancel common divisor */
    seq_t g = gcd(num, denom);
    return denom / g;
}
