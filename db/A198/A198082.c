/*
 * A198082  Ceiling(n*Sqrt(5)).
 * https://oeis.org/A198082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9619 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed as: if n==0 return 0 else floor_sqrt(5*n^2) + 1 */

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
        return (seq_t)0;

    /* n^2 */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn;

    /* 5 * n^2 */
    prod = prod * (seq_t)5;

    /* floor of sqrt(5*n^2) */
    seq_t flo = floor_sqrt(prod);

    /* ceil = floor + 1 for n>0 */
    return flo + (seq_t)1;
}
