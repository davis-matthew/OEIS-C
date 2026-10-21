/*
 * A086655  (binomial(2*p,p)-2)/(2*p) where p runs through the primes.
 * https://oeis.org/A086655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (binomial(2*p, p) - 2) / (2*p) where p = the n-th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A086655.  The function is
   idempotent and can be called with any ordering of n. */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* obtain the n-th prime */
    uint32_t p32 = get_prime(n);          /* p fits in 32‑bit */
    idx_t    p   = (idx_t)p32;

    /* compute binomial(2*p, p) */
    seq_t bin = binomial( (idx_t)(2*p), p );

    /* subtract 2 and divide by 2*p */
    bin -= 2;

    seq_t denom = (seq_t)2 * (seq_t)p;
    seq_t res   = bin / denom;

    return res;
}
