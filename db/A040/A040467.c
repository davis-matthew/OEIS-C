/*
 * A040467  Continued fraction for sqrt(490).
 * https://oeis.org/A040467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = the n-th partial quotient of the continued fraction of
 *            sqrt(490)
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Validated: partial, pass, 90 terms through n = 89 (2026-11-01)
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
    const seq_t D = 490;
    seq_t a0 = floor_sqrt((seq_t)D);     /* floor(sqrt(490)) = 22 */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_val = a0;

    if (n == 0u)
        return a0;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * a_val - m;
        d = (D - m * m) / d;
        a_val = (a0 + m) / d;
    }

    return a_val;
}
