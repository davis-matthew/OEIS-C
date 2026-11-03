/*
 * A186219  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f and g are the triangular numbers and
 *          squares. Complement of A186220.
 * https://oeis.org/A186219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(sqrt((n^2 + n)/2 - 1/4)) (A186219)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 6661 (2026-11-01)
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
    /* handle offset 1: n is at least 1 */
    if (n == 0) return 0;  /* not used, but safe */

    seq_t nn = (seq_t)n;                 /* convert index to seq_t */
    seq_t tmp = nn * nn;                 /* n^2 */
    tmp = tmp * (seq_t)2;                /* 2*n^2 */
    tmp = tmp + nn * (seq_t)2;           /* + 2*n */
    tmp = tmp - (seq_t)1;                /* - 1 */
    seq_t w = floor_sqrt(tmp);           /* floor(sqrt(2*n^2 + 2*n - 1)) */
    seq_t t = w / (seq_t)2;              /* floor(sqrt((2*n^2 + 2*n -1)/4)) */
    seq_t ans = nn + t;                  /* a(n) = n + t */
    return ans;
}
