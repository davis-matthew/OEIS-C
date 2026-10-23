/*
 * A029939  a(n) = Sum_{d|n} phi(d)^2.
 * https://oeis.org/A029939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{d|n} phi(d)^2
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisor enumeration implementation. */
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset starts at 1 */
    seq_t sum = 0;
    idx_t i;
    for (i = 1; i <= n / i; ++i)   /* iterate over divisors up to sqrt(n) */
    {
        if (n % i == 0)
        {
            seq_t p1 = phi(i);
            sum += p1 * p1;
            idx_t j = n / i;
            if (j != i)
            {
                seq_t p2 = phi(j);
                sum += p2 * p2;
            }
        }
    }
    return sum;
}
