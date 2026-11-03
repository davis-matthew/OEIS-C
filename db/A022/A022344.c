/*
 * A022344  Allan Wechsler's "J determinant" sequence.
 * https://oeis.org/A022344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*tau)^2 - n*floor((n+1)*tau) - n^2 where tau =
 *            (1+sqrt(5))/2. Implemented using integer arithmetic and
 *            floor_sqrt
 * Range:     u8 n <= 2, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9717 (2026-11-01)
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
    seq_t k = (seq_t)(n + 1);          /* k = n+1 */
    seq_t sq = (seq_t)5 * k * k;        /* sq = 5*k^2 */
    seq_t x = floor_sqrt(sq);          /* x = floor(k*sqrt(5)) */
    seq_t m = (x + k) / 2;             /* m = floor(k*phi) */
    seq_t ans = m * m - n * m - n * n; /* a(n) = m^2 - n*m - n^2 */
    return ans;
}
