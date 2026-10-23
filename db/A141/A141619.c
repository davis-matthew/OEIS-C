/*
 * A141619  a(n) = 3*(n+1)/2 if n is odd, a(n) = prime(n/2) if n is even.
 * https://oeis.org/A141619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*(n+1)/2 if n is odd, a(n) = prime(n/2) if n is even
 * Range:     u8 n <= 109, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based. */
    if (n & 1ULL)                       /* odd index */
    {
        seq_t x = (seq_t)(n + 1ULL);    /* n+1 is even, so integer division is safe */
        return (x / 2) * 3;             /* compute (n+1)/2 * 3 */
    }
    else                                /* even index */
    {
        idx_t k = n >> 1;               /* n/2 */
        return (seq_t)get_prime(k);     /* get the k‑th prime (1‑based) */
    }
}
