/*
 * A129701  Difference between successive primes cubed: a(n) = prime(n+1)^3 -
 *          prime(n)^3.
 * https://oeis.org/A129701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)^3 - prime(n)^3
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 889, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n < 1 we return 0. */
    if (n < 1) return 0;

    /* Get the nth and (n+1)th primes. */
    seq_t p  = (seq_t)get_prime(n);
    seq_t q  = (seq_t)get_prime(n + 1);

    /* Compute the cubes of the primes. */
    seq_t p3 = p * p * p;
    seq_t q3 = q * q * q;

    /* Return the difference of the cubes. */
    return q3 - p3;
}
