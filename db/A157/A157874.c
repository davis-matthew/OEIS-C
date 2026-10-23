/*
 * A157874  Expansion of 104*x^2 / (-x^3+675*x^2-675*x+1).
 * https://oeis.org/A157874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   c(1)=0, c(2)=104, c(3)=675*c(2), c(n)=675*(c(n-1)-c(n-2))+c(n-3)
 *            for n>3
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for NULL */

seq_t a(idx_t n)
{
    /* Handle indices below the sequence offset (unused). */
    if (n < 1) return 0;

    /* Base cases. */
    if (n == 1) return 0;
    if (n == 2) return (seq_t)104;
    if (n == 3) return (seq_t)675 * (seq_t)104;  /* 70200 */

    /* Iteratively compute up to n. */
    seq_t c_n_minus_3 = (seq_t)0;          /* c(1) */
    seq_t c_n_minus_2 = (seq_t)104;        /* c(2) */
    seq_t c_n_minus_1 = (seq_t)675 * (seq_t)104;  /* c(3) */
    seq_t c_n;                            /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        c_n = (seq_t)675 * (c_n_minus_1 - c_n_minus_2) + c_n_minus_3;
        /* shift for next iteration */
        c_n_minus_3 = c_n_minus_2;
        c_n_minus_2 = c_n_minus_1;
        c_n_minus_1 = c_n;
    }

    return c_n;
}
