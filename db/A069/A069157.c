/*
 * A069157  Number of positive divisors of n that are divisible by the
 *          smallest prime that divides n.
 * https://oeis.org/A069157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = d(n) * e_n / (e_n + 1) where d(n) is the number of
 *            positive divisors of n and e_n is the exponent of the smallest
 *            prime factor of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) is defined as 0 */
    if (n <= 1)
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;
    uint64_t m = nn;
    uint64_t divisor_count = 1;
    uint64_t e = 0;          /* exponent of smallest prime factor */

    /* trial‑division factorisation */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            uint64_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            if (e == 0)          /* this is the first (hence smallest) prime */
                e = exp;
            divisor_count *= (exp + 1);
        }
    }

    /* If remaining part is a prime > sqrt(original n) */
    if (m > 1)
    {
        /* exponent is 1 */
        if (e == 0)          /* n itself was prime */
            e = 1;
        divisor_count *= 2;   /* (1+1) */
    }

    /* a(n) = d(n) * e / (e + 1)  */
    uint64_t result = divisor_count * e / (e + 1);
    return (seq_t)result;
}
