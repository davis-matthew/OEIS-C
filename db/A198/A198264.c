/*
 * A198264  Round(n*sqrt(10)).
 * https://oeis.org/A198264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(10))
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 8706 (2026-11-01)
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
    /* m = 10 * n^2 */
    seq_t nn = (seq_t)n;
    seq_t m  = nn * nn * (seq_t)10;

    /* k = floor(sqrt(m)) */
    seq_t k = floor_sqrt(m);

    /* Compute k^2 and the threshold k^2 + k + 1 */
    seq_t k2       = k * k;
    seq_t threshold = k2 + k + (seq_t)1;

    /* Decide whether to round up */
    seq_t res = k + (seq_t)(m >= threshold);
    return res;
}
