/*
 * A194106  Sum{floor(j*sqrt(3)) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for sqrt(3).
 * https://oeis.org/A194106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} ⌊j·√3⌋
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: ⌊j·√3⌋ = ⌊√(3·j²)⌋, so we can use integer square‑root. */
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
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j)
    {
        seq_t jj = (seq_t)j;      /* j as seq_t */
        seq_t v  = 3 * jj * jj;   /* 3·j², safe because seq_t is big enough for n up to ~10⁶ */
        v = floor_sqrt(v);        /* floor(√(3·j²)) = floor(j·√3) */
        sum += v;
    }
    return sum;
}
