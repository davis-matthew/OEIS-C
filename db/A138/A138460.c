/*
 * A138460  a(n) = ((n-th prime)^6-(n-th prime)^5)/2.
 * https://oeis.org/A138460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^6 - (p_n)^5) / 2 where p_n is the n-th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* index 0 is outside the defined offset */
    uint32_t p = get_prime(n);       /* fetch the n-th prime (1‑based) */
    if (p == 2)                      /* special case for the prime 2 */
        return (seq_t)16;            /* 2^6-2^5 = 64-32 = 32; /2 = 16 */

    /* Compute p^5 = p * p * p * p * p */
    seq_t p5 = (seq_t)p;
    for (int i = 0; i < 4; ++i)
        p5 *= (seq_t)p;           /* p5 now holds p^5 */

    /* Multiply by (p-1)/2.  For odd primes (p-1) is even. */
    seq_t half = (seq_t)((p - 1) / 2);

    return p5 * half;             /* This equals (p^6 - p^5)/2 */
}
