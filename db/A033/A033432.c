/*
 * A033432  a(n) = floor(1000/sqrt(n)).
 * https://oeis.org/A033432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1000 / sqrt(n))
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 8940 (2026-11-01)
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
    /* 1000^2 = 1,000,000 */
    const seq_t denom = 1000000;
    /* n ≥ 1 (offset 1) */
    seq_t q = denom / (seq_t)n;       /* floor(denom / n) */
    seq_t k = floor_sqrt(q);         /* floor(sqrt(q)) = floor( sqrt(denom / n) ) */
    return k;
}
