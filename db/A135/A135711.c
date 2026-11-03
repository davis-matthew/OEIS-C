/*
 * A135711  Minimal perimeter of a polyhex with n cells.
 * https://oeis.org/A135711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*ceil(sqrt(12*n-3))
 * Range:     u8 n <= 5, u16 n <= 1118, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9802 (2026-11-01)
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
    if (n == 0) return 0;          /* out of range, but harmless */
    seq_t x = (seq_t)12 * (seq_t)n - 3; /* 12*n - 3 */
    seq_t s = floor_sqrt(x);          /* floor(sqrt(x)) */
    if (s * s < x) s++;               /* ceil(sqrt(x)) */
    return 2 * s;                     /* final perimeter */
}
