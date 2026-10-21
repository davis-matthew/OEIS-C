/*
 * A229454  Number of defective 3-colorings of an n X 3 0..2 array connected
 *          horizontally and vertically with exactly one mistake and colors
 *          introduced in row-major 0..2 order.
 * https://oeis.org/A229454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n‑1) - 29*a(n‑2) + 20*a(n‑3) - 4*a(n‑4) (n≥5)
 *            Initial values: a(1)=2, a(2)=20, a(3)=140, a(4)=863
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined for n<1, safe default */

    /* Base cases */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)140;
    if (n == 4) return (seq_t)863;

    /* a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a1 = (seq_t)2;
    seq_t a2 = (seq_t)20;
    seq_t a3 = (seq_t)140;
    seq_t a4 = (seq_t)863;
    seq_t val;

    for (idx_t i = 5; i <= n; ++i) {
        val = (seq_t)10 * a4  -  (seq_t)29 * a3
              + (seq_t)20 * a2 -  (seq_t)4  * a1;
        /* shift the window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = val;
    }
    return a4;
}
