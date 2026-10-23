/*
 * A141078  a(n) = abs(prime(n)*prime(n+4) - prime(n+1)*prime(n+3)).
 * https://oeis.org/A141078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = abs(prime(n) * prime(n+4) - prime(n+1) * prime(n+3))
 * Range:     u8 n <= 8, u16 n <= 52, u32 n <= 8017, u64 n <= 8017,
 *            u128 n <= 8017, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  The function is safe for n >= 1.  */
    if (n == 0) return 0;                 /* guard against misuse */

    /* Fetch the five needed primes.  get_prime uses 1‑based indexing. */
    uint32_t p0 = get_prime(n);
    uint32_t p1 = get_prime(n + 1);
    uint32_t p3 = get_prime(n + 3);
    uint32_t p4 = get_prime(n + 4);

    /* Compute the two products in seq_t to avoid overflow. */
    seq_t prod1 = (seq_t)p0 * (seq_t)p4;
    seq_t prod2 = (seq_t)p1 * (seq_t)p3;

    /* Return the absolute difference. */
    return (prod1 > prod2) ? (prod1 - prod2) : (prod2 - prod1);
}
