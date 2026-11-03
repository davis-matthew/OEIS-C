/*
 * A127723  Floor of square root of sum of squares of the first n consecutive
 *          even numbers.
 * https://oeis.org/A127723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(4 * sum_{k=1..n} k^2)) = floor(sqrt( (n)*(n +
 *            1)*(2*n + 1) * 4 / 6 ))
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-01)
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
    /* the sequence is defined for n ≥ 1; a(0) is undefined but
       returning 0 for safety when called with n == 0. */
    if (n == 0)
        return (seq_t)0;

    seq_t N = (seq_t)n;
    seq_t prod = N * (N + (seq_t)1);          /* n*(n+1) */
    prod *= (2 * N + (seq_t)1);              /* * (2n+1) */
    prod *= (seq_t)4;                        /* * 4 */
    prod /= (seq_t)6;                        /* / 6 */
    return floor_sqrt(prod);
}
