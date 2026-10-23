/*
 * A168014  Sum of all parts of all partitions of n into equal parts that do
 *          not contain 1 as a part.
 * https://oeis.org/A168014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * (d(n) - 1), where d(n) is the number of positive
 *            divisors of n. a(0) = 0
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t divisor_count(idx_t n)
{
    /* Return the number of positive divisors of n. n >= 1. */
    seq_t cnt = 1;
    idx_t tmp = n;
    /* factor 2 */
    if ((tmp & 1) == 0)
    {
        int exp = 0;
        while ((tmp & 1) == 0)
        {
            tmp >>= 1;
            exp++;
        }
        cnt *= (seq_t)(exp + 1);
    }
    /* factor odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2)
    {
        if (tmp % p == 0)
        {
            int exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                exp++;
            }
            cnt *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1)            /* remaining prime factor */
        cnt *= 2;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t dcnt = divisor_count(n);
    /* dcnt >= 1, so dcnt-1 fits in seq_t. */
    seq_t result = (seq_t)n * (dcnt - 1);
    return result;
}
