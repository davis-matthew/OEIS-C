/*
 * A077119  a(n) = A077118(n) - n^3.
 * https://oeis.org/A077119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = nearest square to n^3 minus n^3
 * Range:     i8 n <= 3, i16 n <= 25, i32 n <= 56, i64 n <= 56, i128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-01)
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
    /* compute n^3, all in seq_t to keep precision */
    seq_t cube = (seq_t)n * (seq_t)n * (seq_t)n;

    /* nearest square to cube: floor square root of cube */
    seq_t s   = floor_sqrt(cube);
    seq_t s2  = s * s;           /* floor square */
    seq_t s1  = s + 1;
    seq_t s1sq = s1 * s1;       /* next square */

    /* choose the closer square */
    seq_t diff1 = cube - s2;     /* >= 0 */
    seq_t diff2 = s1sq - cube;   /* >= 0 */

    if (diff1 <= diff2)
        return s2 - cube;
    else
        return s1sq - cube;
}
