/*
 * A073072  Minimum value of abs(n^2-x^3) x>0.
 * https://oeis.org/A073072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min_{x>0} |n^2 - x^3|
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_cube_root.h"

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
