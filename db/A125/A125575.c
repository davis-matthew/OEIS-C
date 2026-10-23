/*
 * A125575  Initial digit of squares of primes.
 * https://oeis.org/A125575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A000030(A001248(n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the n‑th prime (1‑indexed) */
    uint32_t p = get_prime(n);
    /* compute its square in a 64‑bit integer */
    uint64_t sq = (uint64_t)p * (uint64_t)p;
    /* extract the most significant decimal digit */
    uint64_t d = sq;
    while (d >= 10U)
        d /= 10U;
    return (seq_t)d;
}
