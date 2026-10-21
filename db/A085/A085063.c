/*
 * A085063  a(n) is the minimal number k such that n+k and n*k+1 are primes.
 * https://oeis.org/A085063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥ 1 such that n + k and n*k + 1 are prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for n = 0, return 0 as a sentinel */
        return 0;

    for (idx_t k = 1; ; ++k)
    {
        idx_t sum  = n + k;
        idx_t prod = n * k + 1;   /* product fits in 64 bits for all used n */

        if (is_prime(sum) && is_prime(prod))
            return (seq_t)k;
    }
}
