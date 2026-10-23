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
 * Helpers:   floor4throot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor4throot.h"

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
