/*
 * A131703  a(n) = Sum_{k=1..floor(sqrt(n))} floor(k*sqrt(n)).
 * https://oeis.org/A131703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊√n⌋} ⌊k·√n⌋
 * Range:     u8 n <= 8, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-01)
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

/* Integer square‑root for unsigned 64‑bit values.
   Returns ⌊√n⌋.  The routine works for all n ≤ 2⁶⁴‑1. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)0xFFFFFFFFFFFFFFFFULL, ans = 0;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { ans = 0; low = 1; continue; }
        if (mid <= n / mid)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t m = floor_isqrt_u64(n);          /* m = ⌊√n⌋ */
    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k)
    {
        /* term = ⌊k·√n⌋  = ⌊√(k²·n)⌋ */
        unsigned __int128 prod128 = (unsigned __int128)k * k * n;
        seq_t term = floor_sqrt((seq_t)prod128);
        sum += term;
    }
    return sum;
}
