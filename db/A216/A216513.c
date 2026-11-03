/*
 * A216513  Number of nonnegative integer solutions to the equation x^2 +
 *          6*y^2 = n.
 * https://oeis.org/A216513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9896 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of nonnegative integer solutions to x^2 + 6*y^2 = n
   by brute–force search over x (0 ≤ x ≤ floor(sqrt(n))) leveraging
   floor_sqrt(seq_t). */

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
    seq_t nt = (seq_t)n;
    idx_t sqrt_n = (idx_t)floor_sqrt(nt);

    for (idx_t x = 0; x <= sqrt_n; ++x)
    {
        seq_t x2 = (seq_t)x * (seq_t)x;
        if (x2 > nt) break;
        seq_t r = nt - x2;
        if (r % 6 != 0) continue;
        seq_t y2 = r / 6;
        seq_t y = floor_sqrt(y2);
        if (y * y == y2)
            count += 1;
    }

    return count;
}
