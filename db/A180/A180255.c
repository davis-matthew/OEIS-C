/*
 * A180255  a(n) = n^2 * a(n-1) + n, a(0)=0.
 * https://oeis.org/A180255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * a(n-1) + n, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once

#include <stdbool.h>  /* for consistency with other utilities */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sq = (seq_t)i * (seq_t)i;   /* i^2 */
        res = res * sq + (seq_t)i;        /* a(i) = a(i-1)*i^2 + i */
    }
    return res;
}
