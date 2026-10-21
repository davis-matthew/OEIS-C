/*
 * A050464  a(n) = Sum_{d|n, n/d=3 mod 4} d.
 * https://oeis.org/A050464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{d|n, (n/d) \equiv 3 \pmod{4}} d
 * Range:     u8 n <= 461, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* The function computes a(n) for n >= 1.
   It iterates over all divisor pairs d, n/d and
   sums d whenever the quotient n/d is congruent to 3 modulo 4. */
seq_t a(idx_t n)
{
    /* n is at least 1, so no guard needed */
    seq_t sum = 0;
    idx_t d;

    for (d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d1 = d;
            idx_t d2 = n / d;  /* the paired divisor */

            /* For divisor d1, the quotient is d2 */
            if ((d2 & 3) == 3)  /* d2 % 4 == 3 */
                sum += (seq_t)d1;

            if (d1 != d2)      /* avoid double counting squares */
            {
                /* For divisor d2, the quotient is d1 */
                if ((d1 & 3) == 3)
                    sum += (seq_t)d2;
            }
        }
    }
    return sum;
}
