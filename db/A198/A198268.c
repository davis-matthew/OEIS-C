/*
 * A198268  Round(n*sqrt(12)).
 * https://oeis.org/A198268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(12))
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9318, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9318 (2026-11-01)
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
    /* Compute x = 12 * n * n */
    seq_t nn = (seq_t)n;
    seq_t x  = nn * nn;   /* n^2   */
    x *= (seq_t)12;       /* 12*n^2 */

    /* s = floor(sqrt(x)) */
    seq_t s = floor_sqrt(x);

    /* Compute 4*x and (2*s + 1)^2 to decide rounding */
    seq_t four_x        = x * ((seq_t)4);
    seq_t two_s_plus_1  = (s << 1) + ((seq_t)1);
    seq_t right         = two_s_plus_1 * two_s_plus_1;

    /* If 4*x >= (2*s+1)^2, bump up to s+1 */
    return (four_x >= right) ? s + ((seq_t)1) : s;
}
