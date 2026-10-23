/*
 * A138222  a(n) = the largest divisor of n that is <= the number of positive
 *          divisors of n.
 * https://oeis.org/A138222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the largest divisor of n that is <= the number of
 *            positive divisors of n (tau(n))
 * Range:     u8 n <= 9565, u16 n <= 9565, u32 n <= 9565, u64 n <= 9565,
 *            u128 n <= 9565, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Computes A138222: a(n) = largest divisor d of n with d <= tau(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined below offset, but safe */

    /* Factor n to compute tau(n) */
    idx_t m = n;
    idx_t tau = 1;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (m > 1)          /* remaining prime factor */
        tau *= 2;

    /* Find the largest divisor <= tau by decreasing search */
    for (idx_t d = tau; d >= 1; --d)
    {
        if (n % d == 0)
            return (seq_t)d;
    }
    return (seq_t)0; /* unreachable, keeps compiler happy */
}
