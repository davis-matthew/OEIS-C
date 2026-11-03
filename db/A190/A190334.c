/*
 * A190334  a(n) = n + floor(n*r/t) + floor(n*s/t) where r=1, s=sqrt(3),
 *          t=1/s.
 * https://oeis.org/A190334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + floor(n*sqrt(3))
 * Range:     u8 n <= 4, u16 n <= 70, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9935 (2026-11-01)
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

/* Computes the OEIS sequence A190334:
 *   a(n) = 4*n + floor(n*sqrt(3))
 * for any n >= 1 (index starts at 1).  The computation uses
 * integer arithmetic only; the floor of n*sqrt(3) is obtained as
 * floor(sqrt(3*n^2)) via the provided floor_sqrt routine.  All
 * intermediate values are cast to seq_t to respect the build‑time
 * integer width or big‑int class. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t prod = 3 * nn * nn;     /* 3*n^2  (may overflow if seq_t is 128‑bit, but within bounds for typical use) */
    seq_t flr = floor_sqrt(prod); /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    return 4 * nn + flr;          /* 4*n + floor(n*sqrt(3)) */
}
