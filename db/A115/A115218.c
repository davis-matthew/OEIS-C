/*
 * A115218  Triangle read by rows: zeroth row is 0; to get row n >= 1, append
 *          next 2^n numbers to end of previous row.
 * https://oeis.org/A115218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 757, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115218 – Triangle read by rows.
 *
 *   Row 0: 0
 *   Row n≥1: append the next 2^n consecutive integers to the previous row.
 *
 *   The sequence is obtained by concatenating all rows.
 *   For an index n, let m be the row number such that
 *
 *       start(m) = ∑_{i=0}^{m-1} (2^{i+1} - 1) = 2^{m+1} - m - 2
 *
 *   satisfies start(m) ≤ n < start(m+1).  Then
 *
 *       a(n) = n - start(m)
 *
 *   This implementation iteratively finds the row containing n.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t start = 0;     /* start index of the current row   */
    idx_t powv  = 1;     /* 2^m                               */

    while (1) {
        /* length of the current row = 2^{m+1} - 1 = 2*powv - 1 */
        idx_t len   = (powv << 1) - 1;
        idx_t next  = start + len;    /* start index of the next row   */
        if (n < next)               /* n lies in the current row     */
            break;
        start = next;
        powv <<= 1;                  /* advance to next row            */
    }

    return (seq_t)(n - start);       /* position within the row        */
}
