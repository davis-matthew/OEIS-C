/*
 * A174468  a(n) = Sum_{d|n} d*sigma(n/d)*sigma(d).
 * https://oeis.org/A174468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} d*sigma(n/d)*sigma(d)
 * Range:     u8 n <= 7, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sum of divisors of m (sigma(m)) using a basic divisor loop. */
static seq_t sigma_u64(uint64_t m)
{
    seq_t sum = 0;
    for (uint64_t i = 1; i <= m / i; ++i)
    {
        if (m % i == 0)
        {
            sum += (seq_t)i;
            if (i != m / i)
                sum += (seq_t)(m / i);
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t N = (uint64_t)n;            /* cast idx_t to a native integer */
    for (uint64_t d = 1; d <= N / d; ++d)
    {
        if (N % d == 0)
        {
            uint64_t d2 = N / d;
            seq_t sigma_d  = sigma_u64(d);
            seq_t sigma_d2 = sigma_u64(d2);
            sum += (seq_t)d * sigma_d2 * sigma_d;
            if (d != d2)
                sum += (seq_t)d2 * sigma_d * sigma_d2;
        }
    }
    return sum;
}
