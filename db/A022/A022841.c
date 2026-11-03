/*
 * A022841  Beatty sequence for sqrt(7).
 * https://oeis.org/A022841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(7))
 * Range:     u8 n <= 3, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9420 (2026-11-01)
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
    if (n == 0) return 0;                /* offset starts at 1 */
    seq_t un = (seq_t)n;                  /* promote idx_t to seq_t */
    seq_t sq = un * un;                   /* n*n  (fits for tested ranges) */
    seq_t prod = sq * (seq_t)7;           /* 7*n*n */
    return floor_sqrt(prod);              /* floor(sqrt(7*n*n)) = floor(n*sqrt(7)) */
}
