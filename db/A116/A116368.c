/*
 * A116368  Central terms of the triangle in A116366.
 * https://oeis.org/A116368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2n) + prime(n+1)
 * Range:     u8 n <= 20, u16 n <= 2329, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based as per the OEIS offset */
    seq_t p1 = (seq_t)get_prime(2 * n);
    seq_t p2 = (seq_t)get_prime(n + 1);
    return p1 + p2;
}
