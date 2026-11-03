/*
 * A073072  Minimum value of abs(n^2-x^3) x>0.
 * https://oeis.org/A073072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min_{x>0} |n^2 - x^3|
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑positive index */

    /* Compute n^2 in seq_t */
    seq_t nsq = (seq_t)n * (seq_t)n;

    /* Find the largest integer x with x^3 <= n^2 */
    seq_t x = floor_cube_root(nsq);
    if (x == 0) x = 1;             /* x must be positive */

    /* Compute |n^2 - x^3| */
    seq_t x3 = x * x * x;
    seq_t diff1 = nsq > x3 ? nsq - x3 : x3 - nsq;

    /* Candidate x+1 */
    seq_t x2 = x + 1;
    seq_t x23 = x2 * x2 * x2;
    seq_t diff2 = x23 > nsq ? x23 - nsq : nsq - x23;

    /* Minimum difference */
    return diff1 < diff2 ? diff1 : diff2;
}
