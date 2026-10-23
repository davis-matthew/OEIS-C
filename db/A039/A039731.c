/*
 * A039731  a(n) = max{prime(n) mod q, where prime q < prime(n) = n-th prime}.
 * https://oeis.org/A039731
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p - nextprime(⌊p/2⌋), where p is the n‑th prime
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 2, so values for n < 2 are irrelevant. */
    if (n < 2)
        return (seq_t)0;

    /* p = the n-th prime (1‑based) */
    uint32_t p = get_prime(n);

    /* Find the smallest prime > ⌊p/2⌋ */
    uint64_t np = next_prime((uint64_t)(p / 2));

    /* a(n) = p - nextprime(p/2) */
    return (seq_t)p - (seq_t)np;
}
