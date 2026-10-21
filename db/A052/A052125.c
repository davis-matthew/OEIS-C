/*
 * A052125  a(n) = n/A034684(n).
 * https://oeis.org/A052125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / d(n) where d(n) is the smallest unitary divisor > 1
 *            of n (i.e., the smallest prime power p^e dividing n)
 * Range:     u8 n <= 513, u16 n <= 9934, u32 n <= 9934, u64 n <= 9934,
 *            u128 n <= 9934, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;       /* a(1) = 1 by definition */

    uint64_t tmp = (uint64_t)n;
    uint64_t min_pow = (uint64_t)-1;   /* largest possible value */

    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            uint64_t pow = 1;
            while (tmp % p == 0)
            {
                tmp /= p;
                pow *= p;
            }
            if (pow < min_pow)
                min_pow = pow;
        }
    }

    /* If a prime factor > sqrt(n) remains, it is a single power */
    if (tmp > 1)
    {
        if (tmp < min_pow)
            min_pow = tmp;
    }

    /* For prime powers min_pow == n */
    return (seq_t)(n / min_pow);
}
