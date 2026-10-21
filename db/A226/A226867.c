/*
 * A226867  Number of n X 3 (-1,0,1) arrays of determinants of 2 X 2 subblocks
 *          of some (n+1) X 4 binary array with rows and columns of the latter
 *          in lexicographically nondecreasing order.
 * https://oeis.org/A226867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 12*a(n-1) -
 *            66*a(n-2) + 220*a(n-3) - 495*a(n-4) + 792*a(n-5) * - 924*a(n-6)
 *            + 792*a(n-7) - 495*a(n-8) + 220*a(n-9) * - 66*a(n-10) +
 *            12*a(n-11) - a(n-12) for n > 13 * Initial values (n = 1 … 13): *
 *            7, 37, 187, 792, 2866, 9136, 26267, 69311, 170084, * 3...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 27, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Initial terms a(1) .. a(13) */
    static const seq_t init[13] = {
        7, 37, 187, 792, 2866, 9136, 26267,
        69311, 170084, 392159, 856558, 1784149, 3563340
    };

    if (n <= 13) {
        return init[n - 1];
    }

    /* Buffer holds the last 12 terms used in the recurrence.
     * At the start of the loop (n = 14) we need a(2) … a(13). */
    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i) {
        buf[i] = init[i + 1];
    }

    seq_t next;
    for (idx_t i = 14; i <= n; ++i) {
        /* Compute next term from the 12 preceding ones */
        next = 12 * buf[11]      /* a(n-1)   */
             - 66 * buf[10]      /* a(n-2)   */
             + 220 * buf[9]      /* a(n-3)   */
             - 495 * buf[8]      /* a(n-4)   */
             + 792 * buf[7]      /* a(n-5)   */
             - 924 * buf[6]      /* a(n-6)   */
             + 792 * buf[5]      /* a(n-7)   */
             - 495 * buf[4]      /* a(n-8)   */
             + 220 * buf[3]      /* a(n-9)   */
             - 66 * buf[2]       /* a(n-10)  */
             + 12 * buf[1]       /* a(n-11)  */
             - buf[0];           /* a(n-12)  */

        /* Shift buffer left, drop oldest term, append new one */
        for (idx_t j = 0; j < 11; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[11] = next;
    }

    /* after the loop 'next' is the last value computed */
    return next;
}
