/*
 * A194111  Sum{floor(j*sqrt(7)) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for sqrt(7).
 * https://oeis.org/A194111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{j=1}^{n} \lfloor j\sqrt{7}\rfloor
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
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
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j)
    {
        seq_t jj  = (seq_t)j;
        seq_t val = floor_sqrt((seq_t)7 * jj * jj);
        sum += val;
    }
    return sum;
}
