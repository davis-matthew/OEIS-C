/*
 * A211159  Number of integer pairs (x,y) such that 0<x<y<=n and x*y=n+1.
 * https://oeis.org/A211159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(d(n+1)/2) – 1, where d(m) is the number of divisors
 *            of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;

    idx_t m = n + 1u;            /* m = n + 1 */
    idx_t temp = m;
    idx_t tau  = 1u;             /* divisor count of m */

    for (idx_t p = 2u; p * p <= temp; ++p)
    {
        if (temp % p == 0u)
        {
            idx_t exp = 0u;
            do
            {
                temp /= p;
                ++exp;
            } while (temp % p == 0u);
            tau *= (exp + 1u);
        }
    }
    if (temp > 1u)               /* remaining prime factor */
        tau *= 2u;

    seq_t res = (seq_t)(tau / 2u) - 1u;
    return res;
}
