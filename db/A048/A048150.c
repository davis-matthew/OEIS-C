/*
 * A048150  a(n)=number of numbers h^2+k^2 that are <=2n^2; equivalently,
 *          a(n)=T(n,n), array T as in A048149.
 * https://oeis.org/A048150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(h,k)∈ℕ₀² : h² + k² ≤ 2 n²} (OEIS A048150)
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* 2*n² in arbitrary‑precision arithmetic */
    seq_t limit = (seq_t)2 * (seq_t)n * (seq_t)n;

    /* maximum possible h, stored in idx_t (fits because n fits idx_t) */
    seq_t hmax_seq = floor_sqrt(limit);
    idx_t hmax = (idx_t)hmax_seq;

    seq_t count = 0;

    for (idx_t h = 0; h <= hmax; ++h) {
        seq_t hsq = (seq_t)h * (seq_t)h;
        seq_t rest = limit - hsq;
        /* maximum k for this h */
        seq_t kmax = floor_sqrt(rest);
        /* k ranges from 0 to kmax inclusive, hence (kmax+1) pairs */
        count += kmax + (seq_t)1;
    }
    return count;
}
