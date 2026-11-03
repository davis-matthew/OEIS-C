/*
 * A187329  Floor((3-sqrt(5))n).
 * https://oeis.org/A187329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((3-sqrt(5))*n)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 120, u64 n <= 120,
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
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t prod = (seq_t)5 * nn * nn;      /* compute 5*n*n */
    seq_t s = floor_sqrt(prod);          /* floor(n*sqrt(5))  */
    /* a(n) = 3*n - ceil(n*sqrt(5)) = 3*n - (s + 1) for n>0 */
    return (seq_t)3 * nn - s - 1;
}
