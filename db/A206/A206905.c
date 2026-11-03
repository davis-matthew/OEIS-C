/*
 * A206905  n+[nr/t]+[ns/t], where []=floor, r=3, s=sqrt(3), t=1/s.
 * https://oeis.org/A206905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*r/t) + floor(n*s/t), with r=3, s=sqrt(3),
 *            t=1/s. * Since t = 1/√3, the second floor becomes 3n and the
 *            first floor becomes * floor(3n√3) = floor(n·√27). Thus a(n) = 4n
 *            + floor(n·√27)
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
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
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn * 27;     /* compute n² * 27 = (n·√27)² */
    seq_t f  = floor_sqrt(sq);   /* floor of the square root = floor(n·√27) */
    return nn * 4 + f;           /* 4n + floor(n·√27) */
}
