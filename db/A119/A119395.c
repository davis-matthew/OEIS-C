/*
 * A119395  Number of nonnegative integer solutions to the equation x^2 + 3y^2
 *          = n.
 * https://oeis.org/A119395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to x^2 + 3y^2 = n
 * Range:     u8 n <= 63, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 118 terms through n = 9523 (2026-11-01)
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
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;
    /* maximum y such that 3y^2 <= n */
    seq_t y_max = floor_sqrt(n_seq / 3);

    for (idx_t y = 0; y <= y_max; ++y)
    {
        seq_t y_sq = (seq_t)y * (seq_t)y;
        seq_t r = n_seq - 3 * y_sq;
        if (r < 0)
            continue;  /* should never happen, but keeps the logic safe */
        seq_t x = floor_sqrt(r);
        if (x * x == r)
            count++;
    }

    return count;
}
