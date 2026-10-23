/*
 * A190726  Central coefficients of Riordan matrix A118384.
 * https://oeis.org/A190726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n,k) * binomial(2*n,n-k) * 2^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    idx_t N = 2 * n;           /* N = 2*n */
    seq_t sum      = 0;        /* accumulator for the sum   */
    seq_t binom_f  = 1;        /* binom_f = binomial(N,0)   */
    seq_t binom_b  = binomial(N, n);  /* binom_b = binomial(N,n)   */
    seq_t pow2 = 1;            /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* term = binomial(N,k) * binomial(N,n-k) * 2^k */
        seq_t term = binom_f * binom_b;
        term = term * pow2;
        sum += term;

        if (k == n) {
            break; /* avoid updating after last iteration */
        }

        /* Update binom_f to binomial(N,k+1):
           binom_f = binom_f * (N - k) / (k + 1) */
        binom_f = binom_f * (N - k) / (k + 1);

        /* Update binom_b to binomial(N,n-(k+1)):
           binom_b = binom_b * (n - k) / (n + k + 1) */
        binom_b = binom_b * (n - k) / (n + k + 1);

        /* Update pow2 to 2^(k+1) */
        pow2 = pow2 * 2;
    }

    return sum;
}
