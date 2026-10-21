/*
 * A231337  Number of (n+1) X (1+1) 0..3 arrays with no element unequal to a
 *          strict majority of its horizontal and vertical neighbors, with
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A231337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + a(n-4) + 3*a(n-5) Initial
 *            values: a(1)=3, a(2)=4, a(3)=7, a(4)=12, a(5)=24
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 28, u64 n <= 54, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the initial terms directly. */
    if (n == 1) return 3;
    if (n == 2) return 4;
    if (n == 3) return 7;
    if (n == 4) return 12;
    if (n == 5) return 24;

    /* Use a sliding window of the last five values. */
    seq_t w[5];
    w[0] = 3;   /* a(1)  */
    w[1] = 4;   /* a(2)  */
    w[2] = 7;   /* a(3)  */
    w[3] = 12;  /* a(4)  */
    w[4] = 24;  /* a(5)  */

    for (idx_t i = 6; i <= n; ++i) {
        /* w[4] = a(i-1), w[3] = a(i-2), w[2] = a(i-3),
           w[1] = a(i-4), w[0] = a(i-5) */
        seq_t next = 3 * w[4]          /* 3*a(i-1) */
                   + w[3]              /* a(i-2)   */
                   - 7 * w[2]          /* -7*a(i-3) */
                   + w[1]              /* a(i-4)   */
                   + 3 * w[0];         /* 3*a(i-5) */

        /* Shift the window for the next iteration. */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = next;
    }

    return w[4];  /* a(n) */
}
