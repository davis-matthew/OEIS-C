/*
 * A017436  a(n) = (11*n + 3)^12.
 * https://oeis.org/A017436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 3)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 3, u128 n <= 147,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    base = base * 11 + 3;        /* 11*n + 3 */

    /* compute the 12th power via repeated squaring */
    seq_t y2 = base * base;      /* (11n+3)^2   */
    seq_t y4 = y2 * y2;          /* (11n+3)^4   */
    seq_t y8 = y4 * y4;          /* (11n+3)^8   */
    return y8 * y4;              /* (11n+3)^12  */
}
