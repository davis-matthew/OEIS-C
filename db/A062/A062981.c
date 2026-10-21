/*
 * A062981  a(n) = n^phi(n).
 * https://oeis.org/A062981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = n^phi(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Compute Euler's totient φ(n) */
    uint64_t phi = (uint64_t)n;
    uint64_t temp = (uint64_t)n;

    for (uint64_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0)
                temp /= p;
            phi -= phi / p;
        }
    }
    if (temp > 1)
        phi -= phi / temp;

    /* Return n^φ(n) */
    return pow_int((seq_t)n, phi);
}
