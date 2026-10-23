/*
 * A048240  Number of new colors that can be mixed with n units of yellow,
 *          blue, red.
 * https://oeis.org/A048240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n} μ(n/d) * (d+1)(d+2)/2 (a(0) = 1)
 * Range:     u8 n <= 22, u16 n <= 366, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    long long sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;            /* first divisor */
            idx_t d2 = n / d;        /* second divisor (n/d) */

            /* term for divisor d1 */
            int mu1 = (int)mobius(d2);   /* μ(n/d1) = μ(d2) */
            long long term1 = ((long long)(d1 + 1) * (d1 + 2)) / 2;
            sum += mu1 * term1;

            if (d1 != d2) /* process the paired divisor only once */
            {
                int mu2 = (int)mobius(d1);   /* μ(n/d2) = μ(d1) */
                long long term2 = ((long long)(d2 + 1) * (d2 + 2)) / 2;
                sum += mu2 * term2;
            }
        }
    }
    return (seq_t)sum;
}
