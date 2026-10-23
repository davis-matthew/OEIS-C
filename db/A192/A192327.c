/*
 * A192327  a(n) = prime(n) mod 2*n.
 * https://oeis.org/A192327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod (2*n)
 * Range:     u8 n <= 54, u16 n <= 7817, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 safely */
    seq_t p = (seq_t)get_prime(n); /* prime(n), fits in 32‑bit */
    uint64_t denom = (uint64_t)2 * n;
    /* If the modulus exceeds the prime, the result is the prime itself. */
    if (denom > (uint64_t)p) return p;
    return p % (seq_t)denom;
}
