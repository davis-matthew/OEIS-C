/*
 * A229366  Number of n X 2 0..2 arrays avoiding 11 horizontally, 22
 *          vertically and 00 antidiagonally.
 * https://oeis.org/A229366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+13*a(n-2)+3*a(n-3)-13*a(n-4)+4*a(n-5) initial
 *            values: a(1)=8, a(2)=38, a(3)=184, a(4)=869, a(5)=4144
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is unused; index 1..5 hold the given initial terms */
    static const seq_t init[6] = {0, 8, 38, 184, 869, 4144};

    if (n <= 5)
        return init[n];

    /* Sliding window of the five most recently computed values:
       a1 = a(n-5), a2 = a(n-4), a3 = a(n-3), a4 = a(n-2), a5 = a(n-1) */
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];

    seq_t cur = 0;
    for (idx_t i = 6; i <= n; ++i) {
        /* the recurrence uses only addition, subtraction and small
           multiplication constants; all intermediate results stay
           non‑negative for n ≥ 6 so unsigned arithmetic is safe. */
        cur = 2 * a5 + 13 * a4 + 3 * a3 - 13 * a2 + 4 * a1;

        /* shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = cur;
    }
    return cur;
}
