/*
 * A141124  Hankel transform of a transform of Jacobsthal numbers.
 * https://oeis.org/A141124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (4n+1) * cos(pi*n/2) + (2n+3) * sin(pi*n/2) Implemented
 *            via periodic table of sin and cos values for multiples of pi/2
 * Range:     i8 n <= 31, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t res;
    switch (n & 3) {               /* n % 4, works also for large n */
        case 0:                    /* cos=1, sin=0 */
            res = (seq_t)4 * (seq_t)n + 1;
            break;
        case 1:                    /* cos=0, sin=1 */
            res = (seq_t)2 * (seq_t)n + 3;
            break;
        case 2:                    /* cos=-1, sin=0 */
            res = -((seq_t)4 * (seq_t)n + 1);
            break;
        case 3:                    /* cos=0, sin=-1 */
            res = -((seq_t)2 * (seq_t)n + 3);
            break;
        default:                   /* unreachable, but keep compiler happy */
            res = 0;
            break;
    }
    return res;
}
