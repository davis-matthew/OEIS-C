/*
 * A179096  Rectified hexateron (5-simplex) numbers: the coefficient of
 *          x^(2n-2) in (1+x+x^2+...+x^(n-1))^6.
 * https://oeis.org/A179096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(2n+3,5) - 6*binom(n+3,5) (OEIS A179096)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 114, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* compute binomial coefficient C(N,5) for N>=0 using safe multiplicative
 * algorithm that keeps intermediate results within seq_t bounds. */
static seq_t binom5(idx_t N)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        seq_t num   = (seq_t)(N - i + 1);   /* numerator part */
        seq_t denom = (seq_t)i;              /* denominator part */

        /* remove common factors between num and denom to minimise overflow */
        seq_t g = gcd(num, denom);
        num   /= g;
        denom /= g;

        /* remove common factors between current result and remaining denom */
        g = gcd(result, denom);
        result /= g;
        denom /= g;

        /* after reductions, denom should be 1; multiply result by reduced numerator */
        result = result * num;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t bin1 = binom5((idx_t)((seq_t)2 * n + 3));
    seq_t bin2 = binom5((idx_t)((seq_t)n + 3));

    seq_t part = bin2 * (seq_t)6;
    return bin1 - part;
}
