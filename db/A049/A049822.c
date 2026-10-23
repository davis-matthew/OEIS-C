/*
 * A049822  a(n) = 1 - tau(n) + Sum_{d|n} tau(d-1).
 * https://oeis.org/A049822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 - tau(n) + Sum_{d|n} tau(d-1)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>   /* for size_t */

/* Compute the number of positive divisors tau(m) for m >= 0.
   For m == 0, tau(0) is defined as 0. */
static seq_t tau_uint64(idx_t m)
{
    if (m == 0)
        return 0;
    seq_t res = 1;
    idx_t x = m;
    for (idx_t p = 2; p <= x / p; ++p)
    {
        if ((x % p) == 0)
        {
            size_t exp = 0;
            while ((x % p) == 0)
            {
                x /= p;
                ++exp;
            }
            res *= (exp + 1);
        }
    }
    if (x > 1)   /* remaining prime factor */
        res *= 2;
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1; safe for n==0 */
        return 0;

    seq_t tau_n = tau_uint64(n);
    seq_t sum = 0;

    for (idx_t i = 1; i <= n / i; ++i)
    {
        if ((n % i) == 0)
        {
            seq_t t1 = tau_uint64(i - 1);
            sum += t1;
            idx_t j = n / i;
            if (j != i)
            {
                seq_t t2 = tau_uint64(j - 1);
                sum += t2;
            }
        }
    }

    seq_t result = sum + 1;   /* 1 - tau(n) + sum_{d|n} tau(d-1) */
    result -= tau_n;
    return result;
}
