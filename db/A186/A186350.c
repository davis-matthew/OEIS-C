/*
 * A186350  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the odd numbers and the
 *          triangular numbers. Complement of A186351.
 * https://oeis.org/A186350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( (sqrt(16*n - 9) - 1) / 2 )
 * Range:     u8 n <= 4, u16 n <= 120, u32 n <= 120, u64 n <= 120,
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
    seq_t temp = ((seq_t)n * ((seq_t)16)) - ((seq_t)9);
    seq_t s = floor_sqrt(temp);
    seq_t add = (s - ((seq_t)1)) / ((seq_t)2);
    return ((seq_t)n) + add;
}
