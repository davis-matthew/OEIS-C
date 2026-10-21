/*
 * A091818  Sum of the even proper divisors of 2n. Sum of the even divisors of
 *          2n that are less than 2n.
 * https://oeis.org/A091818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 9702, u32 n <= 9702, u64 n <= 9702,
 *            u128 n <= 9702, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2 * A001065(n) = sum of the even proper divisors of 2n.
 * Here A001065(n) is the sum of proper divisors of n.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* sum of proper divisors of n */
    unsigned long long sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            if (i != n)
                sum += i;                 /* i < n, always proper divisor */

            idx_t j = n / i;
            if (j != i && j != n)
                sum += j;                 /* counterpart divisor, avoid n */
        }
    }

    /* a(n) = 2 * sum_proper_divisors(n) */
    unsigned long long result = sum * 2ULL;
    return (seq_t)result;
}
