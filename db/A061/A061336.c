/*
 * A061336  Smallest number of triangular numbers which sum to n.
 * https://oeis.org/A061336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest number of triangular numbers whose sum equals n
 *            (0 ≤ a(n) ≤ 3)
 * Range:     u8 n <= 7, u16 n <= 1786, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9925 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

static inline bool is_triangular(seq_t x)
{
    if (x == 0) return true;          /* 0 is T_0 */
    seq_t d = 8 * x + 1;              /* d must be a perfect square */
    seq_t s = floor_sqrt(d);
    if (s * s != d) return false;
    if ((s & 1) == 0) return false;   /* s must be odd, so (s-1)/2 is integer */
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset 0 → a(0) = 0 */

    seq_t sn = (seq_t)n;

    /* 1 triangular number */
    if (is_triangular(sn)) return 1;

    /* 2 triangular numbers: iterate over triangular numbers t = k(k+1)/2 */
    idx_t k = 1;
    seq_t t = 1;                      /* T_1 = 1 */
    for (; t <= sn; ++k, t += k) {
        seq_t rem = sn - t;
        if (is_triangular(rem)) return 2;
    }

    /* 3 triangular numbers – always possible (Gauss's theorem) */
    return 3;
}
