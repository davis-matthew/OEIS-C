/*
 * A052792  Expansion of e.g.f.: x^2*(exp(x)-1)^4.
 * https://oeis.org/A052792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(4^(n-2) - 4*3^(n-2) + 6*2^(n-2) - 4) for n ≥ 6,
 *            a(n)=0 for n<6
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) = 4!*n*(n-1)*Stirling2(n-2,4) with Stirling2(n,4) = (4^n - 4*3^n + 6*2^n - 4)/24 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n < 6) return 0;          /* terms below 6 are zero */

    /* Compute powers using pow_seq; exponent fits in an int for n ≤ 10000 */
    idx_t m = n - 2;               /* m = n-2 */
    seq_t pow4 = pow_seq((seq_t)4, (int)m);
    seq_t pow3 = pow_seq((seq_t)3, (int)m);
    seq_t pow2 = pow_seq((seq_t)2, (int)m);

    /* term = 4^m - 4*3^m + 6*2^m - 4 */
    seq_t term = pow4 -
                 ((seq_t)4 * pow3) +
                 ((seq_t)6 * pow2) -
                 (seq_t)4;

    seq_t coef = (seq_t)n * ((seq_t)(n - 1));
    return coef * term;
}
