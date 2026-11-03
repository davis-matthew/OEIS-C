/*
 * A186512  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-4+5j^2. Complement
 *          of A186511.
 * https://oeis.org/A186512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt(5*n^2 - 5))
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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
    /* Convert the index to seq_t before any arithmetic */
    seq_t sn = (seq_t)n;
    /* Compute 5*n^2 - 5, using seq_t arithmetic to avoid overflow */
    seq_t val = sn * sn * (seq_t)5 - (seq_t)5;
    /* floor_sqrt is provided by the utilities */
    seq_t sqrtv = floor_sqrt(val);
    return sn + sqrtv;
}
