/*
 * A156842  a(n) = 529*n^2 - 746*n + 263.
 * https://oeis.org/A156842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 529*n^2 - 746*n + 263
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 2850, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* use a signed 64‑bit type for intermediate arithmetic to avoid
       unsigned underflow for small n.  The final result is cast to
       seq_t; this is safe because the expression is always non‑negative
       for n >= 1. */
    long long nn = (long long)n;
    long long res = 529LL * nn * nn - 746LL * nn + 263LL;
    return (seq_t)res;
}
