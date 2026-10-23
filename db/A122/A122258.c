/*
 * A122258  Number of Pierpont primes <= n.
 * https://oeis.org/A122258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Pierpont primes <= n
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;         /* No Pierpont primes below 2 */

    seq_t count = 0;
    const idx_t max = n;

    /* 2^u part: iterate over u while 2^u <= max */
    idx_t base = 1;    /* 2^u */
    for (idx_t u = 0; base <= max; ++u)
    {
        /* 3^v part: multiply by powers of 3 while product <= max */
        idx_t pow3 = 1;         /* 3^v */
        while (pow3 <= max / base)
        {
            idx_t val = base * pow3 + 1;   /* candidate = 2^u * 3^v + 1 */
            if (val > max) break;

            if (is_prime(val)) ++count;

            /* Next power of 3 */
            if (pow3 > max / (base * 3)) break; /* avoid overflow */
            pow3 *= 3;
        }

        /* Next power of 2 */
        if (base > (UINT64_MAX >> 1)) break;   /* avoid overflow of left shift */
        base <<= 1;
    }

    return count;
}
