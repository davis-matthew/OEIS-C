/*
 * A182174  a(n) = prime(n)^2 - n.
 * https://oeis.org/A182174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2 - n
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but safe */
    uint32_t p = get_prime(n);
    return (seq_t)p * (seq_t)p - (seq_t)n;
}
