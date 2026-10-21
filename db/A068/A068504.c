/*
 * A068504  Highest power of 2 dividing prime(n)+1.
 * https://oeis.org/A068504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^v₂(pₙ + 1), where pₙ is the n-th prime
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, v2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/v2.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based). */
    seq_t prime = (seq_t)get_prime(n);

    /* Compute exponent of 2 dividing prime + 1. */
    idx_t e = v2(prime + (seq_t)1);

    /* Return the corresponding power of 2. */
    return (seq_t)1 << e;
}
