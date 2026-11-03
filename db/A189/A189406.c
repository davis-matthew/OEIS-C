/*
 * A189406  n+[nr/s]+[nt/s]; r=1, s=-1+sqrt(3), t=1+sqrt(3).
 * https://oeis.org/A189406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n(1+√3)/2⌋ + ⌊n(2+√3)⌋
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-01)
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
    /* For n = 0 (out of range) return 0 just in case. */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;                     /* convert index to seq_t */
    seq_t s  = floor_sqrt( 3 * nn * nn );    /* s = ⌊n·√3⌋ = ⌊√(3n²)⌋   */
    seq_t w  = nn + s;                       /* w = ⌊n(1+√3)⌋ = n + ⌊n√3⌋ */
    seq_t wh = w >> 1;                       /* ⌊w/2⌋ = ⌊n(1+√3)/2⌋      */

    /* a(n) = 2n + w + ⌊w/2⌋  */
    return 2 * nn + w + wh;
}
