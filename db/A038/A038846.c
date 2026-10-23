/*
 * A038846  4-fold convolution of A000302 (powers of 4); expansion of g.f.
 *          1/(1-4*x)^4.
 * https://oeis.org/A038846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+3, 3) * 4^n (OEIS A038846)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"   /* provides seq_t pow4(idx_t exp) */

seq_t a(idx_t n)
{
    /* compute binomial(n+3, 3) = (n+3)(n+2)(n+1)/6 */
    seq_t bin = ((seq_t)(n + 3) * (n + 2) * (n + 1)) / 6;

    /* compute 4^n */
    seq_t pow4n = pow4(n);

    /* return the product */
    return bin * pow4n;
}
