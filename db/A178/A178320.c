/*
 * A178320  INVERT transform of A008805 (triangular numbers repeated).
 * https://oeis.org/A178320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5) (n >= 5)
 *            Initial values: a(0)=1, a(1)=2, a(2)=6, a(3)=14, a(4)=35
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    switch ((idx_t) n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)2;
        case 2: return (seq_t)6;
        case 3: return (seq_t)14;
        case 4: return (seq_t)35;
        default: break; /* for n >= 5 */
    }

    /* Store the last five terms: a[i-5] .. a[i-1] */
    seq_t prev[5];
    prev[0] = (seq_t)1;   /* a(0)   */
    prev[1] = (seq_t)2;   /* a(1)   */
    prev[2] = (seq_t)6;   /* a(2)   */
    prev[3] = (seq_t)14;  /* a(3)   */
    prev[4] = (seq_t)35;  /* a(4)   */

    idx_t i;
    for (i = 5; i <= n; ++i) {
        /* next = 2*prev[4] + 2*prev[3] - 2*prev[2] - prev[1] + prev[0] */
        seq_t next = prev[4];
        next += prev[4];          /* + prev[4]      */
        next += prev[3];
        next += prev[3];          /* + 2*prev[3]    */
        next -= prev[2];
        next -= prev[2];          /* - 2*prev[2]    */
        next -= prev[1];
        next += prev[0];

        /* shift the window: drop prev[0], move others left, insert next */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = next;
    }

    return prev[4];
}
