/*
 * A039712  a(n) = n-th prime modulo 14.
 * https://oeis.org/A039712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod 14
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; get_prime returns the n‑th prime as uint32_t */
    uint32_t p = get_prime(n);
    return (seq_t)(p % 14);
}
