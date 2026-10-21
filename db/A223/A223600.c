/*
 * A223600  Petersen graph (8,2) coloring a rectangular array: number of 2 X n
 *          0..15 arrays where 0..15 label nodes of a graph with edges 0,1 0,8
 *          8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4 3,11 11,13 4,5 4,12
 *          12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and every array movement to
 *          a horizontal, diagonal or ant...
 * https://oeis.org/A223600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2) - 42*a(n-3) - 8*a(n-4) + 48*a(n-5)
 *            Initial values: a(1)=256, a(2)=256, a(3)=1504, a(4)=6736,
 *            a(5)=32768, a(6)=156592
 * Range:     u8 none, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return 0;                     /* below offset */

    if (n == (idx_t)1) return (seq_t)256;
    if (n == (idx_t)2) return (seq_t)256;
    if (n == (idx_t)3) return (seq_t)1504;
    if (n == (idx_t)4) return (seq_t)6736;
    if (n == (idx_t)5) return (seq_t)32768;
    if (n == (idx_t)6) return (seq_t)156592;

    /* last five values: a(n-1), a(n-2), a(n-3), a(n-4), a(n-5) */
    seq_t v0 = (seq_t)156592;  /* a6   */
    seq_t v1 = (seq_t)32768;   /* a5   */
    seq_t v2 = (seq_t)6736;    /* a4   */
    seq_t v3 = (seq_t)1504;    /* a3   */
    seq_t v4 = (seq_t)256;     /* a2   */

    for (idx_t i = (idx_t)7; i <= n; ++i)
    {
        seq_t next = v0 * (seq_t)6
                    + v1 * (seq_t)3
                    + v4 * (seq_t)48
                    - v2 * (seq_t)42
                    - v3 * (seq_t)8;

        /* shift the window for the next iteration */
        v4 = v3;
        v3 = v2;
        v2 = v1;
        v1 = v0;
        v0 = next;
    }

    return v0;
}
