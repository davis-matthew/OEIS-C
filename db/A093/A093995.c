/*
 * A093995  n^2 appears n times, triangle read by rows.
 * https://oeis.org/A093995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor(sqrt(2*n - 1) + 1/2)^2 (OEIS A093995)
 * Range:     u8 n <= 32, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Validated: standard, pass, 82 terms through n = 7052 (2026-11-01)
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

seq_t a(idx_t n) {
    /* n is guaranteed to be >= 1 */
    seq_t S = (seq_t)n * 2 - 1;          /* S = 2*n - 1 */
    seq_t s = floor_sqrt(S);             /* s = floor(sqrt(S)) */
    seq_t t = s * (s + 1) + 1;           /* threshold for rounding up */
    seq_t m = (S >= t) ? (s + 1) : s;    /* nearest integer to sqrt(S) */
    return m * m;                        /* square of the nearest integer */
}
