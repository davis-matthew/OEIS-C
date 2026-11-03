/*
 * A190549  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),4,1) and
 *          []=floor.
 * https://oeis.org/A190549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 153 terms through n = 821 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A190549:
   a(n) = floor((bn+c) * sqrt(2)) - b * floor(n * sqrt(2)) - floor(c * sqrt(2)),
   with r = sqrt(2), b = 4, c = 1.
   This reduces to
   a(n) = floor(sqrt(2*(4n+1)^2)) - 4 * floor(sqrt(2*n^2)) - 1
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
    /* n is at least 1 because the offset is 1 */
    seq_t t = (seq_t)4 * n + (seq_t)1;   /* bn + c = 4n + 1 */
    seq_t t2 = (seq_t)n;                /* n itself */

    seq_t p1 = floor_sqrt((seq_t)2 * t * t);   /* floor(sqrt(2*(4n+1)^2)) */
    seq_t p2 = floor_sqrt((seq_t)2 * t2 * t2); /* floor(sqrt(2*n^2)) */

    return p1 - (seq_t)4 * p2 - (seq_t)1;
}
