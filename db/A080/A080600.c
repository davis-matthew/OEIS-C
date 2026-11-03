/*
 * A080600  a(n) = ceiling(n*(3 + sqrt(13))/2).
 * https://oeis.org/A080600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n*(3 + sqrt(13))/2)
 * Range:     u8 n <= 2, u16 n <= 35, u32 n <= 8947, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8947 (2026-11-01)
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
    if (!n)
        return 0; /* a(0) = 0 */

    seq_t nn = (seq_t)n;               /* convert index to seq_t */
    seq_t s  = floor_sqrt( ((seq_t)13) * nn * nn ); /* floor(n*sqrt13) */
    seq_t p  = 3 * nn + s;              /* 3n + floor(n*sqrt13) */
    seq_t res = p / 2 + 1;              /* floor(p/2) + 1 equals ceil(n*(3+sqrt13)/2) */

    return res;
}
