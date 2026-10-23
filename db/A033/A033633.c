/*
 * A033633  a(n) = n-th prime modulo 19.
 * https://oeis.org/A033633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n) mod 19
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based) */
    uint32_t p = get_prime(n);
    /* return it modulo 19 as a seq_t value */
    return (seq_t)p % (seq_t)19;
}
