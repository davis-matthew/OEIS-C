/*
 * A087056  Difference between 2 * n^2 and the next smaller square number.
 * https://oeis.org/A087056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * n^2 - floor(n * sqrt(2))^2
 * Range:     u8 n <= 5, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
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
    /* Compute 2*n^2 */
    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2 */
    seq_t two_n_sq = nn + nn;           /* 2*n^2 */

    /* k = floor(n * sqrt(2)) = floor(sqrt(2*n^2)) */
    seq_t k = floor_sqrt(two_n_sq);

    /* a(n) = 2*n^2 - k^2 */
    seq_t kk = k * k;
    return two_n_sq - kk;
}
