/*
 * A213556  Principal diagonal of the convolution array A213555.
 * https://oeis.org/A213556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^5 + 15*n^4 + 5*n^3 + n)/30
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 107, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    __int128 tn = n;                          /* promote index to 128‑bit */
    __int128 num = 9 * tn * tn * tn * tn * tn /* n^5 term */
                 + 15 * tn * tn * tn * tn      /* n^4 term */
                 + 5 * tn * tn * tn          /* n^3 term */
                 + tn;                       /* n term */
    return (seq_t)(num / 30);                /* division is exact */
}
