/*
 * A071715  Expansion of (1+x*C^4)*C^4, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*binomial(2*n+3, n)/(n+4) + 8*binomial(2*n+5, n+6)/(n+7)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* first term: 4 * C(2n+3, n) / (n+4) */
    seq_t term1 = binomial(2 * n + 3, n);
    term1 = term1 * 4 / (n + 4);

    /* second term: 8 * C(2n+5, n+6) / (n+7);  binomial is 0 when k>n */
    seq_t term2 = 0;
    if (n + 6 <= 2 * n + 5) {
        term2 = binomial(2 * n + 5, n + 6);
        term2 = term2 * 8 / (n + 7);
    }

    return term1 + term2;
}
