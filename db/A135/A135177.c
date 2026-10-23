/*
 * A135177  a(n) = p^2*(p-1), where p = prime(n).
 * https://oeis.org/A135177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^2*(prime(n)-1)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);
    /* Compute p^2 * (p-1) */
    seq_t p_sq = p * p;
    return p_sq * (p - 1);
}
