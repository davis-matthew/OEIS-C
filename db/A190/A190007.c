/*
 * A190007  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + [n*csc(pi/3)] + [n*(csc(pi/3))^2]
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9389, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 9951 (2026-11-01)
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
    if (n == 0) return 0;   /* offset is 1, but handle 0 safely */

    seq_t n_seq  = (seq_t)n;

    /* term for floor(n*csc(pi/3)) = floor(sqrt(12*n^2)/3) */
    seq_t n_sq        = n_seq * n_seq;          /* n^2   */
    seq_t twelve_n_sq = n_sq * 12;              /* 12*n^2 */
    seq_t sqrt_term   = floor_sqrt(twelve_n_sq);/* floor(sqrt(12*n^2)) */
    seq_t term2       = sqrt_term / 3;          /* floor(n*csc)   */

    /* term for floor(n*(csc(pi/3))^2) = floor(4*n/3) */
    seq_t term3 = (n_seq * 4) / 3;              /* floor(4*n/3)   */

    return n_seq + term2 + term3;
}
