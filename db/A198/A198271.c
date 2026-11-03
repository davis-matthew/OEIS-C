/*
 * A198271  a(n) = round(n*sqrt(13)).
 * https://oeis.org/A198271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(13))
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 9054, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9660 (2026-11-01)
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
    seq_t m  = (seq_t)13 * nn * nn;      /* m = 13·n² = (n·√13)²  */
    seq_t k  = floor_sqrt(m);            /* k = ⌊n·√13⌋          */
    seq_t twok1 = (k << 1) + 1;          /* (2k+1)                */
    seq_t rhs = twok1 * twok1;           /* (2k+1)²                */
    seq_t lhs = m << 2;                  /* 4·m = 52·n²            */
    return (lhs >= rhs) ? k + 1 : k;     /* round to nearest int   */
}
