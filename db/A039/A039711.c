/*
 * A039711  a(n) = n-th prime modulo 13.
 * https://oeis.org/A039711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p_n mod 13, where p_n is the n-th prime (offset 1)
 * Range:     u8 n <= 54, u16 n <= 7081, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Return the n-th prime modulo 13. */
    return ((seq_t)get_prime(n)) % (seq_t)13;
}
