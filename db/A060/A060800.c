/*
 * A060800  a(n) = p^2 + p + 1 where p runs through the primes.
 * https://oeis.org/A060800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 + prime(n) + 1
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 1 using the nth prime. */
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get_prime returns the n-th prime as a uint32_t */
    seq_t p = (seq_t)get_prime(n);
    return p * p + p + (seq_t)1;
}
