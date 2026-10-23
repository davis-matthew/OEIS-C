/*
 * A138909  Expansion of e.g.f.: (1+x)/(1-x*exp(x)).
 * https://oeis.org/A138909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! + Sum_{k=0..n-1} k!*C(n,k)*n*k^(n-k-1) (n>=0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* factorial of n for the final n! term */
    seq_t factn = 1;
    for (idx_t i = 1; i <= n; ++i) factn = factn * (seq_t)i;

    /* running factorial for k!   (k goes from 0 to n-1) */
    seq_t fac = 1;          /* 0! = 1 */

    /* binomial coefficient C(n,k) with k from 0 up to n-1 */
    seq_t binom = 1;        /* C(n,0) = 1 */

    seq_t sum = 0;

    for (idx_t k = 0; k < n; ++k) {
        /* pow = k^(n-k-1)   (big_pow handles 0^0 as 1) */
        seq_t pow_val = big_pow((seq_t)k, (seq_t)(n - k - 1));

        /* term = k! * C(n,k) * n * k^(n-k-1) */
        seq_t term = fac;
        term = term * binom;
        term = term * (seq_t)n;
        term = term * pow_val;

        sum += term;

        /* update binom: C(n,k+1) = C(n,k) * (n-k) / (k+1) */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);

        /* update factorial for next k: (k+1)! */
        fac = fac * (seq_t)(k + 1);
    }

    return factn + sum;
}
