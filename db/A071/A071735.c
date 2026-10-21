/*
 * A071735  Expansion of (1+x^3*C^3)*C^2, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 for n > 0: a(n) = (2*binomial(2*n+1,n) +
 *            5*binomial(2*n-2,n+1)) / (n+2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* 2 * binomial(2*n + 1, n) */
    seq_t t1 = binomial(2*n + 1, n);
    t1 = t1 * (seq_t)2;

    /* 5 * binomial(2*n - 2, n + 1) */
    seq_t t2 = binomial(2*n - 2, n + 1);
    t2 = t2 * (seq_t)5;

    seq_t num = t1 + t2;
    seq_t denom = (seq_t)(n + 2);

    return num / denom;
}
