/*
 * A182617  Number of toothpicks in a toothpick spiral around n cells on
 *          hexagonal net.
 * https://oeis.org/A182617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*n + ceil(sqrt(12*n - 3)) for n > 0, a(0) = 0
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 39 (2026-11-01)
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
    if (n == 0)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t x   = (seq_t)12 * nn - (seq_t)3;      /* 12*n - 3 */
    seq_t s   = floor_sqrt(x);                 /* floor(sqrt(12*n - 3)) */
    seq_t ceil_sqrt = s;

    if (s * s < x)            /* not a perfect square -> ceil */
        ceil_sqrt = s + (seq_t)1;

    return (seq_t)2 * nn + ceil_sqrt;
}
