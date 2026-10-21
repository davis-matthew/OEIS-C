/*
 * A238874  Strictly superdiagonal compositions: compositions (p1, p2, p3,
 *          ...) of n such that pi > i.
 * https://oeis.org/A238874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n = 0) a(n) = Σ_{k≥1, k(k+3)/2 ≤ n} C(n - k(k+3)/2 + k
 *            - 1, k - 1) (n > 0) where C(a, b) is the usual binomial
 *            coefficient
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 103, u64 n <= 259,
 *            u128 n <= 696, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t total = 0;
    for (idx_t k = 1; ; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t kk3_2 = kk * (kk + 3) / 2;
        if (kk3_2 > (seq_t)n)
            break;

        seq_t m = (seq_t)n - kk3_2;              /* m = n - k(k+3)/2 */
        idx_t r = k - 1;                          /* we need C(m + k - 1, k - 1) */

        seq_t bin = 1;
        if (r > 0)
        {
            for (idx_t i = 1; i <= r; ++i)
            {
                seq_t numerator = m + i;
                seq_t denom     = i;

                seq_t g = gcd(bin, denom);
                bin /= g;
                denom /= g;

                g = gcd(numerator, denom);
                numerator /= g;
                denom /= g;   /* denom now 1 */

                bin *= numerator;
            }
        }
        total += bin;
    }
    return total;
}
