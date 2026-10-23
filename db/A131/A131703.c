/*
 * A131703  a(n) = Sum_{k=1..floor(sqrt(n))} floor(k*sqrt(n)).
 * https://oeis.org/A131703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊√n⌋} ⌊k·√n⌋
 * Range:     u8 n <= 8, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

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
