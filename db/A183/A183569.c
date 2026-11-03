/*
 * A183569  n+floor(sqrt(4n-3)), complement of A024206.
 * https://oeis.org/A183569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(4*n - 3))
 * Range:     u8 n <= 16, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Validated: partial, pass, 113 terms through n = 113 (2026-11-01)
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
    seq_t val = (seq_t)n;
    seq_t tmp = val * 4 - 3;
    seq_t sq  = floor_sqrt(tmp);
    return val + sq;
}
