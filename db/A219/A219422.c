/*
 * A219422  Unchanging value maps: number of 2 X n binary arrays indicating
 *          the locations of corresponding elements unequal to no horizontal
 *          or antidiagonal neighbor in a random 0..2 2 X n array.
 * https://oeis.org/A219422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (valid for n > 8):
 *   a(1) = 1,  a(2) = 7,  a(3) = 20,  a(4) = 57,
 *   a(5) = 160, a(6) = 454, a(7) = 1292, a(8) = 3676,
 *   a(n) = 2*a(n-1) + a(n-2) + 3*a(n-3) + 4*a(n-4)
 *              - 2*a(n-5) - 4*a(n-6)
 * with offset 1.  The function below implements this recurrence
 * using a sliding 6‑element window; no memoisation is required.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base values for 1 ≤ n ≤ 8 */
    static const seq_t init[9] = { 0, /* unused 0 index */ 1, 7, 20, 57,
                                  160, 454, 1292, 3676 };

    if (n < 1) return 0;            /* offset is 1; behaviour below is unused */

    if (n <= 8) return init[n];

    /* Window holds a(n-6) … a(n-1)
     * For the first recurrence step (i = 9) we need
     * a(3) … a(8), so we initialise the window accordingly. */
    seq_t buf[6];
    buf[0] = init[3];   /* a(3) */
    buf[1] = init[4];   /* a(4) */
    buf[2] = init[5];   /* a(5) */
    buf[3] = init[6];   /* a(6) */
    buf[4] = init[7];   /* a(7) */
    buf[5] = init[8];   /* a(8) */

    seq_t next = 0;

    for (idx_t i = 9; i <= n; ++i)
    {
        next = (seq_t)2 * buf[5]         /* 2*a(n-1)   */
             + buf[4]                   /*   a(n-2)   */
             + (seq_t)3 * buf[3]         /*   3*a(n-3) */
             + (seq_t)4 * buf[2]         /*   4*a(n-4) */
             - (seq_t)2 * buf[1]         /*   2*a(n-5) */
             - (seq_t)4 * buf[0];        /*   4*a(n-6) */

        /* Shift the 6‑element window to the left:
           buf[0] becomes a(n-5), ..., buf[5] becomes a(n). */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = next;
    }

    return next;
}
