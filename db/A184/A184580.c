/*
 * A184580  a(n) = floor((n-1/4)*sqrt(2)), complement of A184581.
 * https://oeis.org/A184580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1/4)*sqrt(2))
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 5776, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9124 (2026-11-01)
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
    /* Ensure n >= 1; for n == 0 the formula is undefined */

    /* Compute t = 4*n - 1 using seq_t to avoid overflow */
    seq_t t = ((seq_t)n) * (seq_t)4 - (seq_t)1;

    /* Compute u = 2 * t^2 */
    seq_t u = t * t;
    u *= (seq_t)2;

    /* floor_sqrt(u) = floor(sqrt(2*(4*n-1)^2)) = floor((4*n-1)*sqrt(2)) */
    seq_t s = floor_sqrt(u);

    /* Final result: a(n) = floor(s / 4) */
    return s / (seq_t)4;
}
