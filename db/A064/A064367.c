/*
 * A064367  a(n) = 2^n mod prime(n).
 * https://oeis.org/A064367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n mod prime(n)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Prime number indexed by n (1‑based).  get_prime returns uint32_t,
       which is safely cast to seq_t for the modulus. */
    seq_t prime = (seq_t)get_prime(n);
    /* Compute 2^n mod prime using modular exponentiation. */
    return mod_pow((seq_t)2, n, prime);
}
