/*
 * A190886  a(n) = [5nr]-5[nr], where r=sqrt(5).
 * https://oeis.org/A190886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(125*n^2)) - 5*floor(sqrt(5*n^2))
 * Range:     u8 none, u16 n <= 11, u32 n <= 132, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Validated: partial, pass, 132 terms through n = 132 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from a(n) = [5 n sqrt(5)] - 5*[n sqrt(5)] using integer square roots. */
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
    /* Compute n^2 as seq_t to avoid overflow on small types */
    seq_t n_sq = (seq_t)n * (seq_t)n;

    /* floor(sqrt(5  n^2)) */
    seq_t floor5  = floor_sqrt((seq_t)5 * n_sq);

    /* floor(sqrt(125 n^2)) */
    seq_t floor125 = floor_sqrt((seq_t)125 * n_sq);

    /* a(n) = floor125 - 5*floor5 */
    return floor125 - 5 * floor5;
}
