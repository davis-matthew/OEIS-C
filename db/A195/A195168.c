/*
 * A195168  a(n) = 3*n - floor(n*r/3), where r=(1+sqrt(5))/2 (the golden
 *          ratio).
 * https://oeis.org/A195168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - floor(n*(1+sqrt5)/3)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 8606 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses integer arithmetic and the utility floor_sqrt() to
   evaluate floor(n*sqrt5). */
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
    seq_t ns = (seq_t)n;                     /* convert index to seq_t       */
    seq_t temp = (seq_t)5 * ns * ns;          /* 5 * n^2                       */
    seq_t sqrt5n = floor_sqrt(temp);          /* floor(n*sqrt5)                */
    seq_t sum = ns + sqrt5n;                  /* n + floor(n*sqrt5)            */
    seq_t k = sum / (seq_t)6;                 /* floor((n + floor(n*sqrt5))/6) */
    return (seq_t)3 * ns - k;                 /* final value                   */
}
