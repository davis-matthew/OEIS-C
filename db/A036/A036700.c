/*
 * A036700  Number of Gaussian integers z=a+bi satisfying |z|<=n, a>=0,
 *          0<=b<a.
 * https://oeis.org/A036700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Gaussian integers z = a + bi with |z| ≤ n, a ≥
 *            0, 0 ≤ b < a (equivalently, a(n) = Σ_{a=1}^{n} ( min(a-1, ⌊√(n²
 *            – a²)⌋) + 1 ) )
 * Range:     u8 n <= 8, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-01)
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
    seq_t total = 0;
    seq_t n_sq = (seq_t)n * (seq_t)n;

    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        seq_t a_sq = (seq_t)a_val * (seq_t)a_val;
        seq_t m    = n_sq - a_sq;          /* n² – a²  ≥ 0 for a ≤ n   */
        seq_t r    = floor_sqrt(m);        /* ⌊√(n² – a²)⌋           */

        seq_t a_minus_1 = (seq_t)a_val - 1;
        seq_t limit_b   = (a_minus_1 < r) ? a_minus_1 : r;
        total += limit_b + 1;              /* b ranges from 0 to limit_b */
    }

    return total;
}
