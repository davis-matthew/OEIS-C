/*
 * A095875  Number of lattice points on graph of parabola y >= x^2 with y <=
 *          n.
 * https://oeis.org/A095875
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} (2*⌊√k⌋ + 1) (a(-1)=0)
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 55 (2026-11-01)
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
    if (n < 0) return (seq_t)0;           /* a(-1) = 0 */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t m = floor_sqrt((seq_t)k);   /* m = ⌊√k⌋ */
        sum += (seq_t)1 + (seq_t)2 * m;   /* add contribution of y = k */
    }
    return sum;
}
