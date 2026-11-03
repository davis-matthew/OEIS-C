/*
 * A227790  Difference between 3*n^2 and the nearest square number.
 * https://oeis.org/A227790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(3*n^2 - floor(n*sqrt(3))^2, (1+floor(n*sqrt(3)))^2 -
 *            3*n^2)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9946 (2026-11-01)
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

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;             /* offset 1, but keep safe for 0 */
    seq_t nt = (seq_t)n;                     /* cast index to seq_t first */
    seq_t sq = nt * nt;                      /* n^2 */
    sq = sq * 3;                             /* 3*n^2 */

    seq_t k   = floor_sqrt(sq);              /* floor(sqrt(3*n^2)) = floor(n*sqrt(3)) */
    seq_t k2  = k * k;                       /* k^2 */
    seq_t diff1 = sq - k2;                   /* 3*n^2 - k^2 */
    seq_t k1   = k + 1;
    seq_t diff2 = k1 * k1 - sq;              /* (k+1)^2 - 3*n^2 */

    return (diff1 <= diff2) ? diff1 : diff2;
}
