/*
 * A225867  a(n) = max_{2<=k<=(n-2)/2} sum_{d>1: d|n+k, k|n+d}1.
 * https://oeis.org/A225867
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{2 <= k <= (n-2)/2} sum_{d>1, d|n+k, k|n+d} 1
 * Range:     u8 n <= 9618, u16 n <= 9618, u32 n <= 9618, u64 n <= 9618,
 *            u128 n <= 9618, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* sequence is defined for n >= 6 */
    if (n < 6) return 0;

    idx_t max_k = (n - 2) / 2;
    seq_t best = 0;

    /* Iterate over all admissible k */
    for (idx_t k = 2; k <= max_k; ++k)
    {
        idx_t m = n + k;          /* m = n + k */
        seq_t count = 0;

        /* Enumerate all divisors d > 1 of m */
        for (idx_t d = 1; d * d <= m; ++d)
        {
            if (m % d == 0)
            {
                idx_t d1 = d;
                idx_t d2 = m / d;

                if (d1 > 1)
                {
                    /* Check k | n + d1 */
                    if ((n + d1) % k == 0)
                        ++count;
                }

                if (d2 != d1 && d2 > 1)
                {
                    /* Check k | n + d2 */
                    if ((n + d2) % k == 0)
                        ++count;
                }
            }
        }

        if (count > best)
            best = count;
    }

    return best;
}
