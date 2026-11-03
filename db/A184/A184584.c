/*
 * A184584  floor[(n-1/3)r], where r=sqrt(5); complement of A184585.
 * https://oeis.org/A184584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n - 1/3) * sqrt(5))
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: partial, pass, 120 terms through n = 120 (2026-11-01)
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
    /* n is 1‑based; behaviour for n < 1 is undefined. */
    seq_t M = (seq_t)n * 3 - 1;          /* M = 3n - 1 */
    seq_t t = 5 * M * M;                 /* t = 5 * M^2  */
    seq_t s = floor_sqrt(t);             /* s = floor(sqrt(t)) = floor(M * sqrt(5)) */
    return s / 3;                        /* floor((M * sqrt(5)) / 3) */
}
