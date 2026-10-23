/*
 * A183068  Central terms of triangle A183065.
 * https://oeis.org/A183068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n,n) * Σ_{k=0..n} binomial(n,k)^2 *
 *            binomial(2*n+2*k,2*k) * binomial(2*k,k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t i;
    seq_t sum = 0;
    seq_t C = 1;          /* binomial(n,i) */
    seq_t A = 1;          /* binomial(2*n+2*i,2*i) */
    seq_t B = 1;          /* binomial(2*i,i) */

    for (i = 0; i <= n; ++i) {
        seq_t term = C * C;   /* C^2 */
        term = term * A;      /* * A */
        term = term * B;      /* * B */
        sum += term;
        if (i == n) break;

        /* update binomial(n,i+1) */
        C = C * (n - i) / (i + 1);

        /* update binomial(2*n+2*(i+1),2*(i+1)) */
        /*   factor = (2*n+2*i+1)(2*n+2*i+2) / ((2*i+1)(2*i+2)) */
        A = A * ((2*n + 2*i + 1) * (2*n + 2*i + 2))
            / ((2*i + 1) * (2*i + 2));

        /* update binomial(2*(i+1),i+1) */
        /*   factor = (2*i+2)(2*i+1) / ((i+1)^2) */
        B = B * ((2*i + 2) * (2*i + 1))
            / ((i + 1) * (i + 1));
    }

    return sum * binomial(2*n, n);
}
