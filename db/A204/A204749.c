/*
 * A204749  Number of (n+2) X 5 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A204749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) for n > 5 Initial values: a(1)=102,
 *            a(2)=31, a(3)=22, a(4)=27, a(5)=35
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const seq_t init[5] = { 102, 31, 22, 27, 35 };
    if (n <= 5) return init[n - 1];

    seq_t a_prev3 = init[2]; /* a(3) */
    seq_t a_prev2 = init[3]; /* a(4) */
    seq_t a_prev1 = init[4]; /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t curr = a_prev1 + a_prev1; /* 2 * a_prev1 */
        curr -= a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = curr;
    }
    return a_prev1;
}
