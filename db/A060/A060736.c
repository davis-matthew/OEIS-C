/*
 * A060736  Array of square numbers read by antidiagonals in up direction.
 * https://oeis.org/A060736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i^2-j+1 if i>=j, else (j-1)^2+i, where i = n - t*(t+1)/2,
 *            j = ((t*t)+3*t+4)/2 - n, t = floor((sqrt(8*n-7)-1)/2)
 * Range:     u8 n <= 8, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
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
    /* Safety: n >= 1 is assumed by OEIS offset. */
    seq_t nn = (seq_t)n;

    /* t = floor((sqrt(8*n-7)-1)/2) */
    seq_t t = (floor_sqrt(((seq_t)8 * nn) - (seq_t)7) - (seq_t)1) / (seq_t)2;

    /* i = n - t*(t+1)/2 */
    seq_t tri = t * (t + (seq_t)1) / (seq_t)2;
    seq_t i = nn - tri;

    /* j = ((t*t)+3*t+4)/2 - n */
    seq_t num = t * t + (seq_t)3 * t + (seq_t)4;
    seq_t tri2 = num / (seq_t)2;
    seq_t j = tri2 - nn;

    if (i >= j)
        return i * i - j + (seq_t)1;
    else
        return (j - (seq_t)1) * (j - (seq_t)1) + i;
}
