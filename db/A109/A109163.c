/*
 * A109163  a(n) = A019565(n-th prime).
 * https://oeis.org/A109163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes whose indices are the 1‑bits of the
 *            n‑th prime. This implements OEIS A109163: a(n) =
 *            A019565(prime(n))
 * Range:     u8 n <= 8, u16 n <= 30, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Start with product 1 */
    seq_t prod = 1;

    /* Get the n-th prime.  It fits in 64 bits for the sizes we need. */
    uint64_t p = (uint64_t)get_prime(n);

    /* Iterate over the binary bits of p, LSB first. */
    uint64_t bitpos = 1;          /* 1‑based bit index → prime index */
    while (p) {
        if (p & 1ULL)
            prod *= get_prime(bitpos);   /* multiply by the prime at this bit */
        p >>= 1;
        ++bitpos;
    }

    return prod;
}
