/*
 * A036706  a(n)=number of Gaussian integers z=a+bi satisfying n - 1/2 < |z|
 *          <= n + 1/2, a>0, b>=0.
 * https://oeis.org/A036706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 60 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of Gaussian integers a+bi with a>0, b>=0,
   satisfying n‑1/2 < |a+bi| <= n+1/2. */
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

static inline seq_t ceil_isqrt(seq_t x) {
    /* ceil(sqrt(x)) for non‑negative x */
    if (x == (seq_t)0) return (seq_t)0;
    return floor_sqrt(x - (seq_t)1) + (seq_t)1;
}

seq_t a(idx_t n) {
    /* Upper and lower bounds for a^2 + b^2:
       lower = floor((n-0.5)^2) + 1 = n^2 - n + 1
       upper = floor((n+0.5)^2)     = n^2 + n    */
    seq_t N = (seq_t)n;
    seq_t lower = N * N - N + (seq_t)1;
    seq_t upper = N * N + N;

    seq_t count = (seq_t)0;
    /* a ranges from 1 to floor_sqrt(upper) */
    for (idx_t a = (idx_t)1; ; ++a) {
        seq_t a2 = (seq_t)a * (seq_t)a;
        if (a2 > upper) break;

        seq_t max_b2 = upper - a2;
        seq_t bmax = floor_sqrt(max_b2);

        seq_t low_needed = (lower > a2) ? (lower - a2) : (seq_t)0;
        seq_t bmin = (low_needed == (seq_t)0) ? (seq_t)0 : ceil_isqrt(low_needed);

        if (bmax >= bmin) {
            count += (bmax - bmin + (seq_t)1);
        }
    }
    return count;
}
