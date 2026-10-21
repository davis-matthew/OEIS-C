/*
 * A201044  Number of -n..n arrays of 5 elements with adjacent element
 *          differences also in -n..n.
 * https://oeis.org/A201044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) (for n > 6) with initial values a(1)=99, a(2)=1161,
 *            a(3)=6083, a(4)=21141, a(5)=57343, a(6)=131781
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 51, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 6 */
    static const seq_t init[6] = {
        (seq_t)99,
        (seq_t)1161,
        (seq_t)6083,
        (seq_t)21141,
        (seq_t)57343,
        (seq_t)131781
    };

    if (n <= 6)
        return init[n-1];

    /* Keep the last 6 values */
    seq_t prev[6];
    for (int i = 0; i < 6; ++i)
        prev[i] = init[i];

    seq_t cur = 0; /* will hold a(n) after the loop */

    for (idx_t i = 7; i <= n; ++i) {
        /* Recurrence computation */
        cur = (seq_t)6 * prev[5]
             - (seq_t)15 * prev[4]
             + (seq_t)20 * prev[3]
             - (seq_t)15 * prev[2]
             + (seq_t)6  * prev[1]
             -              prev[0];

        /* Shift the window: drop the oldest, append the newest */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = cur;
    }

    return cur;
}
