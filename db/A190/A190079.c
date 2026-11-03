/*
 * A190079  n + [n*s/r] + [n*t/r]; r=1, s=cos(Pi/5), t=sec(Pi/5).
 * https://oeis.org/A190079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·cos(π/5)⌋ + ⌊n·sec(π/5)⌋ Using the identities
 *            cos(π/5) = (√5+1)/4 and sec(π/5) = √5−1, a(n) simplifies to a(n)
 *            = ⌊n·√5⌋ + (n + ⌊n·√5⌋)/4
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8739 (2026-11-01)
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
    /* c = ⌊n·√5⌋  computed as floor(sqrt(5·n²)) */
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t temp = 5 * nn;
    seq_t c = floor_sqrt(temp);

    /* a(n) = c + (n + c) / 4  (integer division) */
    seq_t a_val = c + ((seq_t)n + c) / 4;
    return a_val;
}
