/*
 * A187793  Sum of the deficient divisors of n.
 * https://oeis.org/A187793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, sigma(d) < 2*d} d
 * Range:     u8 n <= 135, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            seq_t dd1 = d1;
            if (sigma_of(d1) < dd1 + dd1)
                sum += dd1;
            idx_t d2 = n / i;
            if (d2 != d1)
            {
                seq_t dd2 = d2;
                if (sigma_of(d2) < dd2 + dd2)
                    sum += dd2;
            }
        }
    }
    return sum;
}
