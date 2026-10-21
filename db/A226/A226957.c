/*
 * A226957  Number of prime divisors p of 2n such that p-1 divides n.
 * https://oeis.org/A226957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ p prime : p | 2n and (p-1) decider divides n }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2 always divides 2n and 2-1=1 divides every n, so always count one */
    if (n == 0) return 0;
    seq_t count = 1;                 /* count for prime 2 */

    uint64_t temp = (uint64_t)n;

    /* remove all factors of 2 from temp */
    while ((temp & 1ULL) == 0)
        temp >>= 1;

    /* test odd prime divisors of n */
    for (uint64_t p = 3; p <= temp / p; p += 2)
    {
        if (temp % p == 0)
        {
            /* p is a prime divisor of n */
            if (n % (p - 1) == 0)
                count++;

            /* divide out all powers of p */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* if n has a remaining prime factor > sqrt(n) */
    if (temp > 1)
    {
        if (n % (temp - 1) == 0)
            count++;
    }

    return count;
}
