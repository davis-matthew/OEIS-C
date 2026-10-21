/*
 * A076025  Expansion of g.f.: (1-3*x*C)/(1-4*x*C) where C = (1 -
 *          sqrt(1-4*x))/(2*x) = g.f. for Catalan numbers A000108.
 * https://oeis.org/A076025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} 3^k * binomial(2*(n-1)+1, (n-1)-k) *
 *            2*(k+1)/(n-1+k+2) (from OEIS: a(n+1) = Sum_{k=0..n}
 *            3^k*binomial(2n+1, n-k)*2*(k+1)/(n+k+2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Handle offset 0 */
    if (n == 0) return (seq_t)1;

    /* For n >= 1, use the formula with n-1 as the parameter in the sum */
    idx_t m = n - 1;          /* m = n-1 */
    seq_t sum = 0;
    seq_t pow3 = 1;           /* 3^k */

    for (idx_t k = 0; k <= m; ++k) {
        /* binomial(2m+1, m-k) */
        seq_t bin = binom(2 * m + 1, m - k);

        /* term = bin * 2*(k+1) / (m + k + 2) */
        seq_t numerator = bin * (seq_t)2 * (seq_t)(k + 1);
        seq_t denominator = (seq_t)(m + k + 2);
        seq_t term = numerator / denominator;   /* exact division */

        term *= pow3;     /* multiply by 3^k */
        sum += term;

        pow3 *= (seq_t)3; /* next power of 3 */
    }

    return sum;
}
