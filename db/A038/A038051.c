/*
 * A038051  G.f.: B(x/(1-x)) where B is g.f. of A000169.
 * https://oeis.org/A038051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n-1, k) * (k+1)^k (OEIS A038051)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined offset, return 0 */
        return 0;

    seq_t sum = 0;                    /* accumulator for a(n) */
    seq_t C   = 1;                    /* C(n-1,0) = 1 */

    for (idx_t k = 0; k < n; ++k) {
        seq_t base   = (seq_t)(k + 1);
        seq_t power  = big_pow(base, (seq_t)k);   /* (k+1)^k */
        sum += C * power;                         /* add term */

        /* update binomial coefficient for next k
           C(n-1,k+1) = C(n-1,k) * (n-1-k) / (k+1)   */
        if (k + 1 < n) {
            C = C * (seq_t)(n - 1 - k) / (seq_t)(k + 1);
        }
    }

    return sum;
}
