/*
 * A174221  a(n) = n/2 if n is even, otherwise add to n the next three primes
 *          > n.
 * https://oeis.org/A174221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n even, a(n) = n/2; * for n odd, a(n) = n + p1 + p2 + p3
 *            where p1,p2,p3 are the first * three primes strictly greater
 *            than n
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Even case: simple division by 2. */
    if ((n & 1ULL) == 0)
        return (seq_t)(n >> 1);

    /* Odd case: sum n with the next three primes > n. */
    uint64_t prime = next_prime((uint64_t)n);   /* first prime > n */
    seq_t result = (seq_t)n + (seq_t)prime;
    for (int i = 1; i < 3; ++i)
    {
        prime = next_prime(prime);              /* next prime > previous one */
        result += (seq_t)prime;
    }
    return result;
}
