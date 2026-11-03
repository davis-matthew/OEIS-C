/*
 * A198272  a(n) = round(n*sqrt(17)).
 * https://oeis.org/A198272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(17))
 * Range:     u8 n <= 0, u16 n <= 15, u32 n <= 3353, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9692 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We compute a(n) without floating point by using the identity
   round(n*sqrt(17)) = (floor(2*n*sqrt(17)) + 1) / 2.  Since
   floor(2*n*sqrt(17)) = floor(sqrt(17 * (2*n)^2)), we can obtain it
   using the integer square‑root routine floor_sqrt. */

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
    seq_t nn = (seq_t)n;          /* n as seq_t */
    seq_t x  = nn * 2;            /* x = 2n */
    seq_t k  = 17 * x * x;        /* k = 17 * (2n)^2 = floor((2n*sqrt(17))^2) */
    seq_t m  = floor_sqrt(k);     /* m = floor(2n*sqrt(17)) */
    seq_t result = (m + 1) / 2;   /* round(n*sqrt(17)) */
    return result;
}
