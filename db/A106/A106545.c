/*
 * A106545  a(n) = n^2 if n^2 is the sum of two primes, otherwise a(n) = 0.
 * https://oeis.org/A106545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 if n^2 can be expressed as a sum of two primes,
 *            otherwise 0. For odd n this reduces to testing whether n^2 - 2
 *            is prime. The is_prime(uint64_t) routine is supplied by the
 *            build environment
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Compute n^2 using seq_t arithmetic */
    seq_t nsq = (seq_t)n * (seq_t)n;

    /* Even n: Goldbach's conjecture guarantees n^2 ≥ 4 is a sum of two primes. */
    if ((n & 1ULL) == 0)
        return nsq;

    /* Odd n: n^2 is odd, so the only representation uses 2 + (n^2 - 2). */
    seq_t m = nsq - (seq_t)2;
    if (is_prime((uint64_t)m))
        return nsq;

    return (seq_t)0;
}
