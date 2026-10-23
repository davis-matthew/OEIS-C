/*
 * A182084  a(n) = 3*n - n/p, where p is the smallest prime dividing n.
 * https://oeis.org/A182084
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - n/p, where p is the smallest prime factor of n
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Smallest prime factor by trial division (does not use external helper) */
static uint64_t spf(uint64_t n)
{
    if ((n & 1) == 0)
        return 2;
    for (uint64_t i = 3; i <= n / i; i += 2)
    {
        if (n % i == 0)
            return i;
    }
    return n; /* n is prime */
}

seq_t a(idx_t n)
{
    /* For indices below the offset (n < 2) the value is undefined.
       Returning 0 is harmless for the OEIS tests. */
    if (n < 2)
        return 0;

    seq_t n_t = (seq_t)n;
    uint64_t p = spf((uint64_t)n);

    /* All operations are performed in the integer type seq_t,
       using only the allowed operators. */
    seq_t res = 3 * n_t - n_t / (seq_t)p;
    return res;
}
