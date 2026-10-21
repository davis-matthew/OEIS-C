/*
 * A233084  Number of 3 X n 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally or antidiagonally, top left element
 *          zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 45*a(n-2) + 27*a(n-3) for n>5 with a(1)=10,
 *            a(2)=122, a(3)=1985, a(4)=32414, a(5)=529862
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)                 /* safety for out‑of‑range calls */
        return 0;
    if (n == 1)
        return (seq_t)10;
    if (n == 2)
        return (seq_t)122;
    if (n == 3)
        return (seq_t)1985;
    if (n == 4)
        return (seq_t)32414;
    if (n == 5)
        return (seq_t)529862U;

    seq_t a_prev1 = (seq_t)529862U;   /* a(n-1) when n==6   */
    seq_t a_prev2 = (seq_t)32414U;
    seq_t a_prev3 = (seq_t)1985U;

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t curr = 19 * a_prev1 - 45 * a_prev2 + 27 * a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = curr;
    }
    return a_prev1;
}
