/*
 * A023645  a(n) = tau(n)-1 if n is odd or tau(n)-2 if n is even.
 * https://oeis.org/A023645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n)-1 if n is odd, otherwise tau(n)-2 * where tau(n)
 *            is the number of positive divisors of n. * This implementation
 *            counts divisors by trial division; * it works efficiently for n
 *            up to at least 10 000
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* not used, but defensive */
        return 0;

    seq_t tau = 0;
    /* count divisors d and n/d for each d <= sqrt(n) */
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            ++tau;                     /* divisor d */
            if (d != n / d)
                ++tau;                 /* paired divisor n/d */
        }
    }

    if (n & 1U)   /* odd */
        return tau - 1;
    else          /* even */
        return tau - 2;
}
