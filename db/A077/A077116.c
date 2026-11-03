/*
 * A077116  n^3 - A065733(n).
 * https://oeis.org/A077116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 - floor(sqrt(n^3))^2 (0 when n is a perfect square)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* cast the index to seq_t for all arithmetic */
    seq_t sn  = (seq_t)n;
    seq_t cube = sn * sn * sn;        /* n^3 in seq_t */
    seq_t s    = floor_sqrt(cube);    /* floor(sqrt(n^3)) */
    return cube - s * s;              /* difference, 0 if n is a square */
}
