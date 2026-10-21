/*
 * A089196  Floor(n / (smallest prime factor of n+1)).
 * https://oeis.org/A089196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / lpf(n+1)) where lpf is the least prime factor
 *            of n+1
 * Range:     u8 n <= 341, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is >= 1; m = n + 1 */
    uint64_t m = (uint64_t)(n + 1);
    if (m <= 2)                     /* m is prime (2) -> a(n)=0 */
        return 0;

    /* Check divisibility by 2 first */
    if ((m & 1) == 0)                /* m even, lpf = 2 */
        return (seq_t)n / 2;

    /* Test odd divisors up to sqrt(m) */
    for (uint64_t d = 3; d <= m / d; d += 2)
    {
        if (m % d == 0)             /* d is the smallest prime factor */
            return (seq_t)n / (seq_t)d;
    }

    /* m is prime */
    return 0;
}
