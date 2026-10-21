/*
 * A223835  Number of n X 5 0..1 arrays with rows, diagonals and antidiagonals
 *          unimodal and columns nondecreasing.
 * https://oeis.org/A223835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) (n > 8) Initial values: a(1)=16, a(2)=86, a(3)=255,
 *            a(4)=596, a(5)=1240, a(6)=2388, a(7)=4325, a(8)=7436
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 126, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)16, (seq_t)86, (seq_t)255, (seq_t)596,
        (seq_t)1240, (seq_t)2388, (seq_t)4325, (seq_t)7436
    };

    if (n <= 0)                      /* not part of the OEIS offset, but keeps the function safe */
        return (seq_t)0;
    if (n <= 8)
        return init[n - 1];

    /* last[0] = a(k-1), last[1] = a(k-2), ..., last[5] = a(k-6)  */
    seq_t last[6];
    for (int i = 0; i < 6; ++i)
        last[i] = init[7 - i];       /* a(8), a(7), a(6), a(5), a(4), a(3) */

    seq_t next = 0;
    for (idx_t k = 9; k <= n; ++k) {
        next = (seq_t)6 * last[0] - (seq_t)15 * last[1] + (seq_t)20 * last[2]
             - (seq_t)15 * last[3] + (seq_t)6 * last[4] - last[5];

        /* shift window for next iteration */
        for (int i = 5; i > 0; --i)
            last[i] = last[i - 1];
        last[0] = next;
    }
    return next;
}
