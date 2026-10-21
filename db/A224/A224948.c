/*
 * A224948  Leap years having 53 Wednesdays and Thursdays.
 * https://oeis.org/A224948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1200 + 400 * floor((n-1) / 14) + off[(n-1)%14] where off
 *            = {392,420,448,476,516,544,572,612,640,668,696,708,736,764}
 *            gives the next leap year that starts on Wednesday
 * Range:     u8 none, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const idx_t off[14] = {392,420,448,476,516,544,572,612,640,668,696,708,736,764};

seq_t a(idx_t n)
{
    /* n >= 1, assume caller follows the offset */
    idx_t i   = (n - 1) % 14;  /* position inside a 400‑year block   */
    idx_t cycle = (n - 1) / 14;/* number of full 400‑year cycles     */

    /* base_start = 1200  (so that year 0 in first cycle is 1592)  */
    seq_t year = (seq_t)1200 + (seq_t)400 * cycle + (seq_t)off[i];
    return year;
}
