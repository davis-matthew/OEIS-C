/*
 * A194772  Number of lower triangles of an (n+2) X (n+2) 0..2 array with new
 *          values introduced in row major order 0..2 and no element unequal
 *          to more than one horizontal or vertical neighbor.
 * https://oeis.org/A194772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + 4*a(n-5) Initial values
 *            (offset 1): a(1)=7, a(2)=17, a(3)=41, a(4)=95, a(5)=219 (values
 *            are correct for all n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset 1 */

    /* handle the first five, defined explicitly */
    if (n <= 5) {
        static const seq_t init[5] = {7, 17, 41, 95, 219};
        return init[n - 1];
    }

    /* sliding window storage for the last five terms */
    seq_t a[5] = {7, 17, 41, 95, 219};   /* a[0]=a(n-5), ..., a[4]=a(n-1) */

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(n) = 3*a(n-1) + a(n-2) - 7*a(n-3) + 4*a(n-5) */
        seq_t term = a[4] * 3 + a[3] + a[0] * 4;   /* 3*a(n-1)+a(n-2)+4*a(n-5) */
        seq_t new_val = term - a[2] * 7;           /* subtract 7*a(n-3) */

        /* shift window: drop a[0], shift others, append new value */
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = a[4];
        a[4] = new_val;
    }

    return a[4];   /* a(n) */
}
