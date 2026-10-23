/*
 * A100479  a(n) = prime(2n-1) + prime(2n).
 * https://oeis.org/A100479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2n-1) + prime(2n)
 * Range:     u8 n <= 15, u16 n <= 1756, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is assumed ≥ 1; for n = 0 we return 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t p1 = get_prime((uint64_t)(2 * n - 1));
    seq_t p2 = get_prime((uint64_t)(2 * n));
    return p1 + p2;
}
