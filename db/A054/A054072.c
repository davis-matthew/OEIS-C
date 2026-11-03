/*
 * A054072  Position of n in the permutation of 1,2,...,n obtained by ordering
 *          the fractional parts {h*sqrt(2)} for h=1,2,...,n.
 * https://oeis.org/A054072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + #{h : 1 ≤ h < n and ⌊n·√2⌋ - ⌊h·√2⌋ ≤ ⌊(n-h)·√2⌋}
 * Range:     u8 n <= 5, u16 n <= 69, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 85 terms through n = 4929 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows from the ordering of fractional parts {h·√2} */

/* Compute ⌊k·√2⌋ by using an integer square root:
   ⌊k·√2⌋ = ⌊√(2·k²)⌋.  */
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
    if (n <= 1U) {
        return 1;   /* a(1) = 1 */
    }

    /* Precompute ⌊n·√2⌋ */
    seq_t fn = floor_sqrt((seq_t)2 * (seq_t)n * (seq_t)n);

    seq_t pos = 1; /* position of n itself */

    for (idx_t h = 1; h < n; ++h) {
        seq_t fh = floor_sqrt((seq_t)2 * (seq_t)h * (seq_t)h);
        seq_t diff = n - h;
        seq_t fl = floor_sqrt((seq_t)2 * (seq_t)diff * (seq_t)diff);

        if ((fn - fh) <= fl) {
            ++pos;
        }
    }

    return pos;
}
