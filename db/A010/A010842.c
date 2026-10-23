/*
 * A010842  Expansion of e.g.f.: exp(2*x)/(1-x).
 * https://oeis.org/A010842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n*a(n-1) + 2^n, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    seq_t prev = 1;        /* a(0) */
    if (n == 0) return prev;

    seq_t pow2 = 1;        /* 2^0 initially */
    for (idx_t k = 1; k <= n; ++k) {
        pow2 *= 2;         /* 2^k */
        prev = (seq_t)k * prev + pow2;
    }
    return prev;
}
