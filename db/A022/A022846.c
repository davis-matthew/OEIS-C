/*
 * A022846  Nearest integer to n*sqrt(2).
 * https://oeis.org/A022846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(2)) = (floor_sqrt(8*n*n)+1)/2
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9896 (2026-11-01)
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
    /* compute 8*n*n using an intermediate 128‑bit type to avoid overflow
       on 64‑bit seq_t builds */
    __int128 t = (__int128)n;
    t = t * t * 8;          /* t = 8*n*n */
    seq_t x = (seq_t)t;     /* convert to the build’s integer type */
    seq_t s = floor_sqrt(x);/* floor(sqrt(x)) */
    return (s + 1) / 2;     /* round to nearest integer */
}
