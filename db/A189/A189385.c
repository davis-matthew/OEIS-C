/*
 * A189385  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=1/sqrt(2), t=1/sqrt(3).
 * https://oeis.org/A189385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3)) + floor(n*sqrt(3/2))
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 992 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (r = 1, s = 1/sqrt(2), t = 1/sqrt(3) ⇒ r/t = sqrt(3), s/t = sqrt(3/2)) */
/* Uses integer square root: n*sqrt(3) = floor(sqrt(3*n^2))  
   and n*sqrt(3/2) = floor(sqrt((3*n^2)/2)). */

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
    if (n == 0)          /* offset is 1, but guard against n = 0 */
        return 0;
    seq_t mn  = (seq_t)n;          /* convert index to seq_t */
    seq_t n2  = mn * mn;           /* n^2  (fits easily in any seq_t) */
    seq_t t3  = 3 * n2;            /* 3 * n^2 */
    seq_t term = mn
               + floor_sqrt(t3);              /* floor(n*sqrt(3)) */
    term += floor_sqrt(t3 / 2);            /* floor(n*sqrt(3/2)) */
    return term;
}
