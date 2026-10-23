/*
 * A031215  Even-indexed primes: a(n) = prime(2n).
 * https://oeis.org/A031215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2n), i.e., the 2n-th prime
 * Range:     u8 n <= 27, u16 n <= 3271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1; harmless fallback */
    idx_t idx = n + n;            /* compute 2*n safely */
    uint32_t p = get_prime(idx);  /* get_prime is 1‑based; get_prime(2n) is required */
    return (seq_t)p;              /* cast to the build's seq_t type */
}
