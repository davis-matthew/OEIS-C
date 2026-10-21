/*
 * A208403  Number of n X 3 0..3 arrays with new values 0..3 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=182, a(3)=10682, a(n)=70*a(n-1)-441*a(n-2) for n>3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)182;
    if (n == 3) return (seq_t)10682;

    seq_t a_prev2 = (seq_t)182;     /* a(2) */
    seq_t a_prev1 = (seq_t)10682;   /* a(3) */
    seq_t a_curr  = a_prev1;        /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        a_curr = (seq_t)70 * a_prev1 - (seq_t)441 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
