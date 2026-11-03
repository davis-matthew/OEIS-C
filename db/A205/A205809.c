/*
 * A205809  G.f.: Sum_{n=-oo..oo} q^(9n^2+4n).
 * https://oeis.org/A205809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if there exists an integer k such that 9k^2+4k = n, 0
 *            otherwise
 * Range:     u8 n <= 1, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 121 terms through n = 120 (2026-11-01)
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
    /* discriminant D = 16 + 36*n must be a perfect square */
    seq_t disc = (seq_t)n;
    disc *= 36;        /* 36*n */
    disc += 16;        /* 16 + 36*n */

    seq_t sq = floor_sqrt(disc);
    if (sq * sq != disc) return 0;   /* D not a square → no integer solution */

    /* Check both signs of the quadratic formula */
    seq_t rem1 = (sq - 4) % 18;   /* corresponds to (-4 + sqrt(D))/18 */
    seq_t rem2 = (sq + 4) % 18;   /* corresponds to (-4 - sqrt(D))/18 */

    if (rem1 == 0 || rem2 == 0) return 1;
    return 0;
}
