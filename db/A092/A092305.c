/*
 * A092305  Length of period of sequence of Genocchi number of first kind read
 *          modulo (2n+1).
 * https://oeis.org/A092305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1)*∏_{p|2*n+1} (p-1)/2
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 2*n + 1, odd */
    seq_t m = (seq_t)n * 2 + 1;
    seq_t result = m;

    /* factor m; m <= 2^64-1 for n in the tested range */
    uint64_t m64 = (uint64_t)m;
    uint64_t temp = m64;

    /* trial division by odd primes starting at 3 */
    for (uint64_t p = 3; p * p <= temp; p += 2)
    {
        if (temp % p == 0)
        {
            /* multiply by (p-1)/2 for each distinct prime factor */
            result *= (seq_t)((p - 1) / 2);
            /* remove all copies of p */
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* if a prime factor larger than sqrt(temp) remains */
    if (temp > 1)
    {
        result *= (seq_t)((temp - 1) / 2);
    }

    return result;
}
