/*
 * A092345  a(0)=1; a(n) = sigma_1(n) + sigma_3(n).
 * https://oeis.org/A092345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma_1(n) + sigma_3(n) where sigma_k(n) is the sum of
 *            the k-th powers of the positive divisors of n. a(0) is defined
 *            to be 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    seq_t sum1 = 0;
    seq_t sum3 = 0;

    /* loop over divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            seq_t dd = (seq_t)d;
            sum1 += dd;
            sum3 += dd * dd * dd;

            /* if d is not the square root, add the paired divisor */
            idx_t e = n / d;
            if (e != d)
            {
                seq_t ee = (seq_t)e;
                sum1 += ee;
                sum3 += ee * ee * ee;
            }
        }
    }

    return sum1 + sum3;
}
