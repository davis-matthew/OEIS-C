/*
 * A117109  Moebius transform of binomial(n+3, 4).
 * https://oeis.org/A117109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} μ(d) * binom(n/d + 3, 4) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 565, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_nplus3_choose4, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"
#include "../utils/binom_nplus3_choose4.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; n≥1 in normal use */
    seq_t result = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t e = n / d;
            int mu_d = mu_of(d);
            if (mu_d > 0)
                result += binom_nplus3_choose4(e);
            else if (mu_d < 0)
                result -= binom_nplus3_choose4(e);

            if (e != d)
            {
                int mu_e = mu_of(e);
                if (mu_e > 0)
                    result += binom_nplus3_choose4(d);
                else if (mu_e < 0)
                    result -= binom_nplus3_choose4(d);
            }
        }
    }
    return result;
}
