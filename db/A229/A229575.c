/*
 * A229575  Number of defective 4-colorings of an n X 5 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly one mistake, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 12*a(n-3) - 4*a(n-4) for n > 7
 *            Initial values: a(1)=20, a(2)=216, a(3)=362, a(4)=568, a(5)=924,
 *            a(6)=1576, a(7)=2852
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, if needed */

seq_t a(idx_t n)
{
    /* Initial terms a(1)..a(7), with a dummy 0 entry for a(0). */
    static const seq_t init[8] = {
        0,          /* unused */
        20,         /* a(1) */
        216,        /* a(2) */
        362,        /* a(3) */
        568,        /* a(4) */
        924,        /* a(5) */
        1576,       /* a(6) */
        2852        /* a(7) */
    };

    /* Return the precomputed value for n ≤ 7. */
    if (n <= 7)
        return init[n];

    /* Start from the known values a(4)..a(7). */
    seq_t a1 = init[7]; /* a(current)   */
    seq_t a2 = init[6]; /* a(prev)      */
    seq_t a3 = init[5]; /* a(prev-1)    */
    seq_t a4 = init[4]; /* a(prev-2)    */

    /* Advance the sequence up to n. */
    for (idx_t i = 8; i <= n; ++i)
    {
        /* Compute next = 6*a1 - 13*a2 + 12*a3 - 4*a4
           All intermediate results are guaranteed to be positive
           for this sequence, so unsigned subtraction is safe. */
        seq_t sum_pos = 6 * a1 + 12 * a3;       /* 6*a1 + 12*a3 */
        seq_t sum_neg = 13 * a2 + 4 * a4;       /* 13*a2 + 4*a4 */
        seq_t next = sum_pos - sum_neg;         /* a(n) */

        /* Shift the window: a4←a3, a3←a2, a2←a1, a1←next. */
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = next;
    }

    /* a1 now holds a(n). */
    return a1;
}
