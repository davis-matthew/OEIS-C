/*
 * A084863  Number of solutions to n = 2*u^2 + 3*v^2, u*v>0.
 * https://oeis.org/A084863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive integer pairs (u,v) with n = 2*u^2 +
 *            3*v^2 (i.e. u > 0, v > 0)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Validated: partial, pass, 102 terms through n = 102 (2026-11-01)
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
    /* sequence starts at n = 1; n = 0 is unused but handled safely */
    if (n == 0) return 0;

    seq_t count = 0;
    for (idx_t u = 1; ; ++u)
    {
        seq_t useq = (seq_t)u;
        seq_t t = useq * useq * 2;          /* 2*u^2 */
        if (t >= (seq_t)n) break;           /* no further solutions possible */

        seq_t rem = (seq_t)n - t;           /* n - 2*u^2   */
        if (rem % 3 != 0) continue;        /* must be multiple of 3 */

        seq_t v2 = rem / 3;                 /* v^2 = rem/3 */
        seq_t v  = floor_sqrt(v2);          /* integer sqrt */

        if (v > 0 && v * v == v2)           /* v must be positive integer square root */
            ++count;
    }
    return count;
}
