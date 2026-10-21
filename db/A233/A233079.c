/*
 * A233079  Number of n X 5 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally or antidiagonally, top left element
 *          zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 118*a(n-1) - 411*a(n-2) + 294*a(n-3); Initial values:
 *            a(1)=41, a(2)=4631, a(3)=529862
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */
    if (n == 1) return (seq_t)41;
    if (n == 2) return (seq_t)4631;
    if (n == 3) return (seq_t)529862;

    seq_t a1 = (seq_t)41;                /* a(i-3) */
    seq_t a2 = (seq_t)4631;              /* a(i-2) */
    seq_t a3 = (seq_t)529862;            /* a(i-1) */
    seq_t a4;

    for (idx_t i = 4; i <= n; ++i) {
        a4 = (seq_t)118 * a3 - (seq_t)411 * a2 + (seq_t)294 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;                           /* now holds a(n) */
}
