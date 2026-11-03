/*
 * A154782  Least integer y>=0 such that n+2y^2 is a square; -1 if n is not in
 *          A035251.
 * https://oeis.org/A154782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest y ≥ 0 such that n + 2*y² is a perfect square;
 *            a(n) = -1 if no such y exists
 * Range:     i8 n <= 18, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 81 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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
    if (n == 0)
        return 0;                           /* n = 0 is a square already */

    idx_t y_limit = (idx_t)floor_sqrt((seq_t)n) + 1;  /* safe bound for y */

    for (idx_t y = 0; y <= y_limit; ++y) {
        seq_t val = (seq_t)n + 2 * (seq_t)y * (seq_t)y;
        seq_t m   = floor_sqrt(val);
        if (m * m == val)                    /* val is a perfect square */
            return (seq_t)y;
    }
    return (seq_t)-1;                       /* no representation found */
}
