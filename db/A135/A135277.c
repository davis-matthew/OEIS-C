/*
 * A135277  a(n) = prime(2n-1) + prime(2n) + prime(2n+1).
 * https://oeis.org/A135277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2*n-1) + prime(2*n) + prime(2*n+1)
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term. The function works for any n >= 1. */
    /* Use idx_t arithmetic for the indices to avoid overflow on n. */
    idx_t i = n;
    idx_t idx1 = i + i - 1;          /* 2*n-1 */
    idx_t idx2 = i + i;              /* 2*n   */
    idx_t idx3 = i + i + 1;          /* 2*n+1 */

    seq_t p1 = (seq_t)get_prime(idx1);
    seq_t p2 = (seq_t)get_prime(idx2);
    seq_t p3 = (seq_t)get_prime(idx3);

    return p1 + p2 + p3;
}
