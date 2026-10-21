/*
 * A209094  Number of n X 2 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than one of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A209094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-2) for n>3 Initial values: a(1)=2,
 *            a(2)=11, a(3)=82
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0)
        return 0;               /* undefined but safe fallback */
    if (n == 1)
        return 2;
    if (n == 2)
        return 11;
    if (n == 3)
        return 82;

    /* Iteratively apply the linear recurrence. */
    seq_t a_prev2 = 11;          /* a(2) */
    seq_t a_prev1 = 82;          /* a(3) */
    seq_t a_curr;
    const seq_t c8   = (seq_t)8;
    const seq_t c4   = (seq_t)4;

    for (idx_t i = 4; i <= n; ++i) {
        a_curr = c8 * a_prev1 - c4 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
