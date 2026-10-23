/*
 * A122186  First row sum of the 4 X 4 matrix M^n, where M={{10, 9, 7, 4}, {9,
 *          8, 6, 3}, {7, 6, 4, 2}, {4, 3, 2, 1}}.
 * https://oeis.org/A122186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) + 21*a(n-2) - 4*a(n-3) - a(n-4) for n >= 4 *
 *            Initial values: a(0) = 1, a(1) = 30, a(2) = 707, a(3) = 16886 *
 *            The implementation uses the recurrence directly; it works for
 *            any * integer type seq_t (including arbitrary–precision) as
 *            required
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms */
    static const seq_t init[4] = { (seq_t)1,
                                   (seq_t)30,
                                   (seq_t)707,
                                   (seq_t)16886 };

    /* handle the first four terms directly */
    if (n < (idx_t)4)
        return init[n];

    /* recurrence variables */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t next = (seq_t)0;

    /* compute terms iteratively up to n */
    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        next = 23 * a3 + 21 * a2 - 4 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return next;
}
