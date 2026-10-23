/*
 * A134338  a(n) = product of the "isolated divisors" of n. A divisor k of n
 *          is isolated if neither k-1 nor k+1 divides n.
 * https://oeis.org/A134338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all divisors d of n such that neither d-1 nor
 *            d+1 divides n (isolated divisors)
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is at least 1 by the problem statement. */
    seq_t prod = (seq_t)1;          /* product, start with multiplicative identity */

    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* Process divisor d1 */
            bool isolated = true;
            if (d1 > 1)           /* avoid division by zero when d1 == 1 */
            {
                if (n % (d1 - 1) == 0)
                    isolated = false;
            }
            if (n % (d1 + 1) == 0)
                isolated = false;
            if (isolated)
                prod *= (seq_t)d1;

            /* Process divisor d2 (if distinct from d1) */
            if (d2 != d1)
            {
                isolated = true;
                if (d2 > 1)
                {
                    if (n % (d2 - 1) == 0)
                        isolated = false;
                }
                if (n % (d2 + 1) == 0)
                    isolated = false;
                if (isolated)
                    prod *= (seq_t)d2;
            }
        }
    }

    return prod;
}
