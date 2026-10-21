/*
 * A236965  Number of nonzero quartic residues modulo the n-th prime.
 * https://oeis.org/A236965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 1 if prime(n) = 2 * (prime(n)-1)/4 if prime(n) ≡ 1 (mod
 *            4) * (prime(n)-1)/2 if prime(n) ≡ 3 (mod 4)
 * Range:     u8 n <= 54, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  The build utilities guarantee get_prime(n) is
       defined for all n >= 1. */
    if (n == 0)
        return (seq_t)0;     /* offset handling – callers never call this */

    /* Retrieve the n‑th prime as a native 32‑bit integer. */
    uint32_t p32 = get_prime(n);

    /* Convert to the sequence type. */
    seq_t p = (seq_t)p32;

    /* Special case for the first prime. */
    if (p == (seq_t)2)
        return (seq_t)1;

    /* For odd primes: use the congruence modulo 4. */
    if ((p & (seq_t)3) == (seq_t)1)
        return (p - (seq_t)1) / (seq_t)4;
    else
        return (p - (seq_t)1) / (seq_t)2;
}
