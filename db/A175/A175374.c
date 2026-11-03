/*
 * A175374  a(n) = A175373(n^2).
 * https://oeis.org/A175374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Count of integer pairs (x,y) with x^4 + y^4 <= n^2, where -n <=
 *            x,y <= n
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find floor of the fourth root of m, searching in [0, limit] */
static seq_t floor4throot(seq_t m, idx_t limit)
{
    seq_t low = 0;
    seq_t high = (seq_t)limit;
    seq_t best = 0;
    while (low <= high)
    {
        seq_t mid = (low + high) >> 1;
        seq_t mid2 = mid * mid;
        seq_t mid4 = mid2 * mid2;
        if (mid4 <= m)
        {
            best = mid;
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t sn = (seq_t)n;
    seq_t nsq = sn * sn;          /* n^2 */

    for (idx_t x = 0; x <= n; ++x)
    {
        seq_t sx = (seq_t)x;
        seq_t sx2 = sx * sx;
        seq_t sx4 = sx2 * sx2;

        if (sx4 > nsq)     /* no solutions for larger x */
            break;

        seq_t rem = nsq - sx4;   /* n^2 - x^4 >= 0 */

        seq_t ymax = floor4throot(rem, n);

        /* Count y from -ymax to ymax */
        seq_t cnt_y = (ymax << 1) + 1;   /* 2*ymax + 1 */

        /* Include both signs of x unless x == 0 */
        seq_t factor_x = (x == 0) ? 1 : 2;

        total += factor_x * cnt_y;
    }
    return total;
}
