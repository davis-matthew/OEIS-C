/*
 * A094181  a(n) = (n - tau(n))*(n - phi(n)), where tau=A000005 and
 *          phi=A000010.
 * https://oeis.org/A094181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - tau(n)) * (n - phi(n)) where tau = number of
 *            divisors, phi = Euler's totient
 * Range:     u8 n <= 23, u16 n <= 317, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

/* auxiliary functions for computing tau(n) and phi(n) by trial division */
static uint64_t divisor_count_u64(uint64_t n)
{
    uint64_t count = 1;
    uint64_t m = n;
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
            count *= (exp + 1);
        }
    }
    if (m > 1)
        count *= 2;
    return count;
}

/* --- required function --- */
seq_t a(idx_t n)
{
    uint64_t n64   = (uint64_t)n;
    seq_t    tn    = (seq_t)n64;
    uint64_t tau   = divisor_count_u64(n64);
    seq_t    phi   = totient_u64(n64);

    seq_t diff1 = tn - (seq_t)tau;
    seq_t diff2 = tn - phi;
    return diff1 * diff2;
}
