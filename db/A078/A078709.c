/*
 * A078709  a(n) = floor(n/d(n)), where d(n) is the number of divisors of n
 *          (A000005).
 * https://oeis.org/A078709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / d(n)), where d(n) is the number of divisors of
 *            n
 * Range:     u8 n <= 260, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard against zero, though the sequence is defined for n >= 1. */
    if (n == (idx_t)0)
        return (seq_t)0;

    idx_t temp = n;
    idx_t divisor_count = 1;
    idx_t cnt = 0;

    /* factor out 2 separately */
    while ((temp & 1u) == 0)
    {
        temp >>= 1u;
        ++cnt;
    }
    if (cnt > 0)
        divisor_count *= (cnt + 1);

    /* trial division by odd numbers */
    for (idx_t p = 3; p <= temp / p; p += 2)
    {
        if (temp % p == 0)
        {
            cnt = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++cnt;
            }
            divisor_count *= (cnt + 1);
        }
    }

    /* if any prime factor > sqrt(original n) remains */
    if (temp > 1)
        divisor_count *= 2;

    /* Return floor(n / d(n)) */
    return (seq_t)n / (seq_t)divisor_count;
}
