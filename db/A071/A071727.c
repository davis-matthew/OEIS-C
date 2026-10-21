/*
 * A071727  Expansion of (1+x^3*C)*C^2, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*binomial(2*n-4,n-3)/n + 2*binomial(2*n+1,n)/(n+2);
 *            Initial values: a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;

    /* For n >= 3 use the explicit formula */
    seq_t bin1 = binomial(2 * n - 4, n - 3);   /* C(2n-4, n-3) */
    seq_t bin2 = binomial(2 * n + 1, n);       /* C(2n+1, n)   */

    seq_t term1 = (seq_t)3 * bin1 / (seq_t)n;
    seq_t term2 = (seq_t)2 * bin2 / ((seq_t)n + (seq_t)2);

    return term1 + term2;
}
