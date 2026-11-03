/*
 * A190704  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),4,2) and [ ]=floor.
 * https://oeis.org/A190704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4*n+2)*sqrt(3)) - 4*floor(n*sqrt(3)) - 3
 * Range:     u8 none, u16 n <= 17, u32 n <= 132, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Validated: partial, pass, 132 terms through n = 132 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = floor((bn+c)*r) - b*floor(n*r) - floor(c*r) with
 *           r = sqrt(3), b = 4, c = 2.  */
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

    /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    seq_t floor_n = floor_sqrt((seq_t)3 * n_seq * n_seq);

    /* floor((4n+2)*sqrt(3)) = floor(sqrt(3*(4n+2)^2)) */
    seq_t t = (seq_t)4 * n_seq + (seq_t)2;
    seq_t floor_t = floor_sqrt((seq_t)3 * t * t);

    /* subtract the fixed parts */
    return floor_t - (seq_t)4 * floor_n - (seq_t)3;
}
