/*
 * A186222  Adjusted joint rank sequence of (g(i)) and (f(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          squares. Complement of A186221.
 * https://oeis.org/A186222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(-1/2 + sqrt(2*n^2))
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8931 (2026-11-01)
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
    seq_t n_seq = (seq_t)n;
    seq_t t = (seq_t)2 * n_seq * n_seq;           /* 2*n^2   */
    seq_t s = floor_sqrt(t);                     /* floor(sqrt(2*n^2)) */

    seq_t L = (seq_t)8 * n_seq * n_seq;           /* 8*n^2   */
    seq_t two_s_plus_one = (seq_t)2 * s + 1;      /* 2*s+1   */
    seq_t D = two_s_plus_one * two_s_plus_one;   /* (2*s+1)^2 */

    seq_t adjust = (L < D) ? 1 : 0;               /* subtract 1 if fractional part < 0.5 */
    return n_seq + s - adjust;
}
