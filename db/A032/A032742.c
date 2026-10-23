/*
 * A032742  a(1) = 1; for n > 1, a(n) = largest proper divisor of n (that is,
 *          for n>1, maximum divisor d of n in range 1 <= d < n).
 * https://oeis.org/A032742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n / smallest_prime_factor(n), with a(1)=1
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest proper divisor of n (for n>1), 1 for n=1 or prime. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1 */
        return 1;

    uint64_t nn = (uint64_t)n;
    uint64_t spf = nn; /* assume prime until proven otherwise */

    /* check divisibility by 2 */
    if ((nn % 2) == 0)
        spf = 2;
    else
    {
        /* trial division by odd numbers up to sqrt(nn) */
        for (uint64_t d = 3; d <= nn / d; d += 2)
        {
            if ((nn % d) == 0)
            {
                spf = d;
                break;
            }
        }
    }

    /* largest proper divisor = n / spf (spf==n when n is prime) */
    return (seq_t)(nn / spf);
}
