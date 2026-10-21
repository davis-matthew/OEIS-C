/*
 * A076273  a(0) = 1, a(1) = 2; for n>1, a(n) = prime(n)+prime(n-1)-1.
 * https://oeis.org/A076273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 2; for n > 1, a(n) = prime(n) + prime(n‑1) – 1
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    /* get_prime(k) returns the kth prime with k starting at 1. */
    seq_t p_n   = (seq_t)get_prime(n);
    seq_t p_nm1 = (seq_t)get_prime(n - 1);

    return p_n + p_nm1 - (seq_t)1;
}
