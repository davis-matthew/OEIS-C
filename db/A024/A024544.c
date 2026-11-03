/*
 * A024544  a(n) = [ 1/{n/sqrt(2)} ], where {x} := x - [ x ].
 * https://oeis.org/A024544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ 1 / { n / √2 } ⌋ where {x} = x – ⌊x⌋. The calculation
 *            is performed using only integer arithmetic
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 116 terms through n = 996 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* 1. Compute M = ⌊ n / √2 ⌋.
       Note that n/√2 = n*√2/2.  The integer part of n*√2/2 can be
       obtained via the integer square‑root of 2*n².  */
    seq_t val = (seq_t)2 * ((seq_t)n * n);          /* 2 * n²  */
    seq_t S   = floor_sqrt(val);                    /* ⌊√(2n²)⌋ */
    seq_t M   = S / 2;                              /* ⌊n/√2⌋   */

    /* 2. Find the largest integer q such that
          q² n² ≤ 2 (1 + q M)² .
       This inequality follows from q ≤ 1/ f, where
          f = { n/√2 } = n/√2 – M.  */
    idx_t low  = 1;
    idx_t high = 4 * n * n;   /* generous upper bound (≈ (2n)²) */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;          /* upper mid */

        unsigned __int128 lhs = (unsigned __int128)mid * mid * n * n;
        unsigned __int128 prod = (unsigned __int128)mid * (unsigned __int128)M;
        unsigned __int128 rhs = (unsigned __int128)2 *
                                ((1 + prod) * (1 + prod));

        if (lhs <= rhs)
            low = mid;
        else
            high = mid - 1;
    }
    return (seq_t)low;
}
