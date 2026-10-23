/*
 * A039710  a(n) = n-th prime modulo 12.
 * https://oeis.org/A039710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod 12
 * Range:     u8 n <= 54, u16 n <= 6618, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Return the n-th prime modulo 12.  The helper get_prime(n)
       yields the n-th prime (1-based). */
    return (seq_t)get_prime(n) % 12;
}
