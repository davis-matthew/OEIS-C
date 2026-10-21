/*
 * A054844  Number of ways to write n as the sum of any number of consecutive
 *          integers (including the trivial one-term sum n = n).
 * https://oeis.org/A054844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*A001227(n) (twice the number of odd divisors of n)
 * Range:     u8 n <= 9883, u16 n <= 9883, u32 n <= 9883, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remove all factors of 2 to obtain the odd part. */
    idx_t odd = n;
    while ((odd & 1) == 0)
        odd >>= 1;

    /* Count divisors of the odd part. */
    seq_t count = 1;
    for (idx_t p = 3; p * p <= odd; p += 2)
    {
        if (odd % p == 0)
        {
            uint32_t exp = 0;
            while (odd % p == 0)
            {
                odd /= p;
                exp++;
            }
            count *= (seq_t)(exp + 1);
        }
    }
    if (odd > 1)          /* remaining prime factor */
        count *= 2;

    /* The sequence value is twice the divisor count. */
    return count * 2;
}
