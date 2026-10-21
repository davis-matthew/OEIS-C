/*
 * A211114  Number of (n+1) X (n+1) -2..2 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one or three distinct values.
 * https://oeis.org/A211114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 6*a(n-3) - 2*a(n-4) + 4*a(n-5)
 *            Initial values: a(1)=9, a(2)=19, a(3)=39, a(4)=81, a(5)=167
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety guard; offset starts at 1 */
    if (n <= 5) {
        static const seq_t init[6] = {0, 9, 19, 39, 81, 167};
        return init[n];
    }

    /* keep the last five values */
    seq_t a1 = 9;    /* a(1) */
    seq_t a2 = 19;   /* a(2) */
    seq_t a3 = 39;   /* a(3) */
    seq_t a4 = 81;   /* a(4) */
    seq_t a5 = 167;  /* a(5) */
    seq_t ai = 0;

    for (idx_t i = 6; i <= n; ++i) {
        /* compute 2*a5 + 3*a4 + 4*a1  minus  6*a3 + 2*a2  */
        seq_t big   = 2 * a5 + 3 * a4 + 4 * a1;
        seq_t small = 6 * a3 + 2 * a2;
        ai = big - small;          /* result stays positive for all n≥6 */

        /* shift window */
        a1 = a2;  a2 = a3;  a3 = a4;  a4 = a5;  a5 = ai;
    }

    return ai;
}
