/*
 * A211513  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2*w^2=x*y.
 * https://oeis.org/A211513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with 1 ≤ w,x,y ≤ n
 *            satisfying 2*w^2 = x*y. Computed by iterating all (x,y) pairs
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    seq_t count = 0;
    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            seq_t prod = (seq_t)x * y;          /* x*y fits in seq_t */
            if ((prod & 1) != 0)                /* product must be even */
                continue;
            seq_t half = prod >> 1;             /* (x*y)/2 */
            seq_t w = floor_sqrt(half);         /* integer sqrt of half */
            if (w * w == half)                  /* check perfect square */
                ++count;                         /* found a valid triple */
        }
    }
    return count;
}
