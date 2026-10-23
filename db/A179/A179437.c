/*
 * A179437  a(n) = |2n - prime(n)|.
 * https://oeis.org/A179437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |2*n - prime(n)|
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    // not used, safe guard
    seq_t two_n = (seq_t)2 * n;               // 2*n as seq_t
    seq_t p      = (seq_t)get_prime(n);       // nth prime
    return two_n > p ? two_n - p : p - two_n;
}
