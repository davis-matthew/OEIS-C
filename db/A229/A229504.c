/*
 * A229504  Number of defective 3-colorings of an n X 2 0..2 array connected
 *          diagonally and antidiagonally with exactly one mistake, and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 6, a(n) = 8*a(n-1) - 16*a(n-2) for n >= 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For indices below the offset behavior does not matter. */
    if (n <= 1) return (seq_t)0;          /* a(1) = 0, a(0) undefined */
    if (n == 2) return (seq_t)6;          /* a(2) = 6 */

    seq_t prev2 = (seq_t)0;  /* a(1) */
    seq_t prev1 = (seq_t)6;  /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 8*a(i-1) - 16*a(i-2) */
        cur = (seq_t)8 * prev1 - (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
