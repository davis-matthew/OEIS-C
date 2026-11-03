/*
 * A185546  a(n) = floor((1/2)*(n+1)^(3/2)); complement of A185547.
 * https://oeis.org/A185546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1/2)*(n+1)^(3/2))
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 650, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 78 terms through n = 4993 (2026-11-01)
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
    seq_t m   = (seq_t)n + 1;   /* m = n+1 */
    seq_t rad = m * m * m;      /* m^3 */
    seq_t s   = floor_sqrt(rad);/* floor(sqrt(m^3)) = floor((n+1)^(3/2)) */
    return s / 2;               /* floor(.../2) */
}
