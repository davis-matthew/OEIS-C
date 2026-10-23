/*
 * A100099  An inverse Chebyshev transform of x/(1-2x).
 * https://oeis.org/A100099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} C(n,k)*(2^(n-2k)-0^(n-2k))/2 (Here 0^0
 *            is treated as 1, so the term is zero when n-2k==0.)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum = 0;
    seq_t binom = 1;  /* C(n,0) */

    for (idx_t k = 0; k <= n / 2; ++k) {
        idx_t m = n - 2 * k;               /* exponent of 2 minus 1 */
        if (m > 0) {
            seq_t p = pow2(m - 1);         /* 2^(m-1) */
            sum += binom * p;              /* term contribution */
        }
        if (k == n / 2) break;             /* last k: m==0 -> no term */

        /* Update binom to C(n,k+1) */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return sum;
}
