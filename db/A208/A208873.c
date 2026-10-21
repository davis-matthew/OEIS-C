/*
 * A208873  Number of 2 X n 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=12, a(3)=136, a(4)=1798, a(5)=24472; for n>5:
 *            a(n)=18*a(n-1)-65*a(n-2)+84*a(n-3)-36*a(n-4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  // required by the build

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* value irrelevant below the offset */

    /* first five terms */
    static const seq_t init[5] = {2, 12, 136, 1798, 24472};
    if (n <= 5) return init[n - 1];

    /* recurrence for n > 5 */
    seq_t a2 = 12;    /* a(n-2) */
    seq_t a3 = 136;   /* a(n-3) */
    seq_t a4 = 1798;  /* a(n-4) */
    seq_t a5 = 24472; /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t sum1 = 18 * a5 + 84 * a3;  /* 18*a(n-1) + 84*a(n-3) */
        seq_t sum2 = 65 * a4 + 36 * a2;  /* 65*a(n-2) + 36*a(n-4) */
        seq_t t   = sum1 - sum2;         /* a(n) */
        a2 = a3;   /* shift window */
        a3 = a4;
        a4 = a5;
        a5 = t;
    }
    return a5;
}
