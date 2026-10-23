/*
 * A163467  a(n) = floor(p/2) * floor(floor(p/2)/2) *
 *          floor(floor(floor(p/2)/2)/2) * ... * 1, where p=prime(n).
 * https://oeis.org/A163467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(p/2)*floor(floor(p/2)/2)*...*1, where p = the nth
 *            prime (OEIS A000040)
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 76, u64 n <= 549,
 *            u128 n <= 5000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Obtain the nth prime.  get_prime returns uint32_t, cast to seq_t. */
    seq_t p = (seq_t)get_prime(n);

    /* Accumulate the product of successive right‑shifts of p. */
    seq_t prod = 1;
    while (p > 1) {
        p /= 2;      /* floor(p/2) */
        prod *= p;   /* multiply by the current residual */
    }
    return prod;
}
