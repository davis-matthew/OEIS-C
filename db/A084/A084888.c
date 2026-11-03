/*
 * A084888  Number of partitions of n^3 into two squares>0.
 * https://oeis.org/A084888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered positive solutions to * x^2 + y^2 =
 *            n^3 * Implemented by brute‑force scan of the smaller square
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 118 terms through n = 970 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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
    if (n == 0)
        return (seq_t)0;

    /* n³ */
    seq_t nn    = (seq_t)n;
    seq_t n3    = nn * nn * nn;

    /* Upper bound for the smaller of the two squares:
     *   x² ≤ y²  ⇒  2x² ≤ n³  ⇒  x ≤ √(n³ / 2)   */
    seq_t half      = n3 / (seq_t)2;
    seq_t limit_sq  = floor_sqrt(half);
    idx_t  limit    = (idx_t)limit_sq;

    seq_t cnt = 0;
    for (idx_t x = 1; x <= limit; ++x)
    {
        seq_t x2  = (seq_t)x * (seq_t)x;
        seq_t rem = n3 - x2;                /* y² = n³ – x²   */
        seq_t y   = floor_sqrt(rem);
        if (y * y == rem)                   /* perfect square?*/
            ++cnt;
    }
    return cnt;
}
