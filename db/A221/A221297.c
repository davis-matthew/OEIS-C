/*
 * A221297  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal or vertical neighbor, with no
 *          occupancy greater than 2.
 * https://oeis.org/A221297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 27*a(n-2) - a(n-3) + 8*a(n-4) - 16*a(n-5) -
 *            6*a(n-6) + 2*a(n-7), with a(1)=3, a(2)=19, a(3)=137, a(4)=1001,
 *            a(5)=7305, a(6)=53291, a(7)=388741
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* sequence offset is 1 */

    /* Initial values for n = 1 … 7 */
    static const seq_t base[7] = {
        (seq_t)3,
        (seq_t)19,
        (seq_t)137,
        (seq_t)1001,
        (seq_t)7305,
        (seq_t)53291,
        (seq_t)388741
    };

    if (n <= 7)
        return base[n - 1];

    /* Temporarily store the last seven terms */
    seq_t prev[7];
    for (int i = 0; i < 7; ++i)
        prev[i] = base[i];

    for (idx_t i = 8; i <= n; ++i) {
        seq_t val = (seq_t)11 * prev[6];   /* 11*a(i-1) */
        val -= (seq_t)27 * prev[5];        /* -27*a(i-2) */
        val -= prev[4];                    /* -a(i-3)   */
        val += (seq_t)8  * prev[3];        /* +8*a(i-4) */
        val -= (seq_t)16 * prev[2];        /* -16*a(i-5)*/
        val -= (seq_t)6  * prev[1];        /* -6*a(i-6) */
        val += (seq_t)2  * prev[0];        /* +2*a(i-7) */

        /* Shift the window of previous terms */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = prev[6];
        prev[6] = val;
    }

    return prev[6];
}
