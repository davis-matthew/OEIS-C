/*
 * A135178  a(n) = p^3 + p^2 where p = prime(n).
 * https://oeis.org/A135178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^3 + p^2 where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint32_t p = get_prime(n);          /* p is the n-th prime */
    seq_t p_t = (seq_t)p;
    return p_t * p_t * (p_t + (seq_t)1);
}
