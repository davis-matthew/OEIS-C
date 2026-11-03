/*
 * A179125  a(n) = A000037(n)^3.
 * https://oeis.org/A179125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (nth positive integer that is not a perfect square)^3
 * Range:     u8 none, u16 n <= 34, u32 n <= 1585, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 7873 (2026-11-01)
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
    if (n == 0) return 0;                    /* offset 1 – undefined for n=0 */

    /* Binary search for the n‑th non‑square integer. */
    idx_t low  = 1;
    idx_t high = n + 1000; /* safe upper bound for n ≤ 10000 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t mid_sq = floor_sqrt((seq_t)mid);     /* ⌊√mid⌋   */
        seq_t cnt    = (seq_t)mid - mid_sq;        /* non‑squares ≤ mid */

        if (cnt >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }

    seq_t x  = (seq_t)low;
    seq_t x2 = x * x;
    return x2 * x;  /* x³ */
}
