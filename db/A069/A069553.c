/*
 * A069553  Define S(k) to be the sequence of divisors and multiples of k,
 *          e.g. S(1) = 1,2,3,4,5... S(2) = 1,2,4,6,8,10,... S(10) =
 *          1,2,5,10,20,30,40,50,...; a(n) = n-th term of the n-th sequence
 *          S(n).
 * https://oeis.org/A069553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n - d(n) + 1) where d(n) = number of divisors of n
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t divisor_count_u64(idx_t n)
{
    idx_t cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            if (i * i == n)
                ++cnt;
            else
                cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n==0 we return 0. */
    if (n == 0)
        return (seq_t)0;

    idx_t d = divisor_count_u64(n);
    seq_t nn = (seq_t)n;
    seq_t dn = (seq_t)d;
    return nn * (nn - dn + (seq_t)1);
}
