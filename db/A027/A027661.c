/*
 * A027661  Fractional part of decimal expansion of sqrt(n) to 3 places.
 * https://oeis.org/A027661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(1000*sqrt(n)+0.5) mod 1000
 * Range:     u8 none, u16 none, u32 n <= 49, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 65 terms through n = 8073 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes the rounded fractional part of sqrt(n) to 3 decimal places */
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
    /* M = n * 1_000_000  (i.e. (sqrt(n)*1000)^2) */
    seq_t M = (seq_t)n * 1000000ULL;

    /* integer sqrt: floor(sqrt(M)) */
    seq_t r = floor_sqrt(M);

    /* round up if sqrt(M) >= r + 0.5 i.e. if 4*M >= (2*r+1)^2 */
    seq_t left  = 4 * M;
    seq_t two_r_plus1 = (r << 1) + 1;          /* 2*r + 1 */
    seq_t right = two_r_plus1 * two_r_plus1;

    seq_t rounded = (left >= right) ? r + 1 : r;

    /* keep only the fractional part: modulo 1000 */
    return rounded % 1000;
}
