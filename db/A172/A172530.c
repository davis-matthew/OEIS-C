/*
 * A172530  Number of ways to place 3 nonattacking knights on an n X n
 *          toroidal board.
 * https://oeis.org/A172530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^4 - 27*n^2 + 218)/6 (n≥6). Small n are handled by
 *            hard‑coded values: a(1)=0, a(2)=0, a(3)=6, a(4)=208, a(5)=600
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 53, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    switch (n) {
        case 1:  return 0;
        case 2:  return 0;
        case 3:  return 6;
        case 4:  return 208;
        case 5:  return 600;
        default:
        {
            seq_t nn = (seq_t)n;
            seq_t nn2 = nn * nn;            /* n^2   */
            seq_t nn4 = nn2 * nn2;          /* n^4   */
            seq_t term = nn4 - (seq_t)27 * nn2 + 218;  /* n^4 - 27*n^2 + 218 */
            seq_t num  = nn2 * term;        /* n^2 * ( ... ) */
            return num / 6;                 /* divide by 6 */
        }
    }
}
