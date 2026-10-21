/*
 * A234135  Number of (n+1) X (3+1) 0..3 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of all six edge and diagonal
 *          differences equal to 9.
 * https://oeis.org/A234135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) + 8*a(n-2) – 30*a(n-3) + 4*a(n-4) + 48*a(n-5) – 32*a(n-6);
 * initial values: a(1)=192, a(2)=296, a(3)=488, a(4)=968, a(5)=1992, a(6)=4808
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the first six terms directly */
    if (n == 1) return (seq_t)192;
    if (n == 2) return (seq_t)296;
    if (n == 3) return (seq_t)488;
    if (n == 4) return (seq_t)968;
    if (n == 5) return (seq_t)1992;
    if (n == 6) return (seq_t)4808;

    /* use the linear recurrence for n >= 7 */
    seq_t a1 = (seq_t)192;   /* a(n-6)   */
    seq_t a2 = (seq_t)296;   /* a(n-5)   */
    seq_t a3 = (seq_t)488;   /* a(n-4)   */
    seq_t a4 = (seq_t)968;   /* a(n-3)   */
    seq_t a5 = (seq_t)1992;  /* a(n-2)   */
    seq_t a6 = (seq_t)4808;  /* a(n-1)   */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t an = 3 * a6 + 8 * a5 - 30 * a4 + 4 * a3 + 48 * a2 - 32 * a1;
        /* shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = an;
    }
    return a6;
}
