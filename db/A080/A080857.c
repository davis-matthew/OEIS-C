/*
 * A080857  a(n) = (25*n^2 - 15*n + 2)/2.
 * https://oeis.org/A080857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*n^2 - 15*n + 2)/2, offset 0
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t term = nn * nn;          /* n^2 */
    term = term * 25;              /* 25*n^2 */
    term = term - (15 * nn) + 2;   /* 25*n^2 - 15*n + 2 */
    term = term / 2;               /* divide by 2 */
    return term;
}
