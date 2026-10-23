/*
 * A106519  a(n) = (2/n)*binomial(2*n-2, n-1) - (1/(2*n))*Sum_{d|n}
 *          Moebius(d)*binomial(2*n/d, n/d).
 * https://oeis.org/A106519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/n)*binomial(2*n-2, n-1) - (1/(2*n))*Sum_{d|n}
 *            Moebius(d)*binomial(2*n/d, n/d)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Offset starts at 1. */

    seq_t nn = (seq_t)n;
    seq_t two_n = (seq_t)2 * nn;

    /* First term: (2/n) * C(2n-2, n-1) */
    seq_t bin1 = binomial((idx_t)(2 * n - 2), (idx_t)(n - 1));
    seq_t term1 = ( (seq_t)2 * bin1 ) / nn;

    /* Sum over divisors: Σ μ(d) * C(2n/d, n/d) */
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            int mu1 = mobius(d1);
            if (mu1 != 0) {
                idx_t k1 = (idx_t)(2 * n / d1);
                idx_t r1 = (idx_t)(n / d1);
                seq_t bin = binomial(k1, r1);
                if (mu1 == 1) sum += bin;
                else sum -= bin;            /* mu1 == -1 */
            }

            if (d1 != d2) {
                int mu2 = mobius(d2);
                if (mu2 != 0) {
                    idx_t k2 = (idx_t)(2 * n / d2);
                    idx_t r2 = (idx_t)(n / d2);
                    seq_t bin = binomial(k2, r2);
                    if (mu2 == 1) sum += bin;
                    else sum -= bin;            /* mu2 == -1 */
                }
            }
        }
    }

    /* Second term: (1/(2n)) * Σ μ(d) * C(2n/d, n/d) */
    seq_t term2 = sum / two_n;

    return term1 - term2;
}
