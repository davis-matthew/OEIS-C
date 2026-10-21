/*
 * A223558  Petersen graph (3,1) coloring a rectangular array: number of 3 X n
 *          0..5 arrays where 0..5 label nodes of a graph with edges 0,1 0,3
 *          3,5 3,4 1,2 1,4 4,5 2,0 2,5 and every array movement to a
 *          horizontal or antidiagonal neighbor moves along an edge of this
 *          graph, with the array starting at 0.
 * https://oeis.org/A223558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17*a(n-1) - 47*a(n-2) + 41*a(n-3) - 10*a(n-4) with a(1) =
 *            36, a(2) = 243, a(3) = 3249, a(4) = 44217, a(5) = 609309, a(6) =
 *            8410671
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1 */

    /* initial six values */
    static const seq_t base[7] = {
        0,          /* unused, index starts at 1 */
        36,         /* a(1) */
        243,        /* a(2) */
        3249,       /* a(3) */
        44217,      /* a(4) */
        609309,     /* a(5) */
        8410671      /* a(6) */
    };

    if (n <= 6) {
        return base[n];
    }

    /* iterate from 7 up to n using the recurrence
       only four previous states need to be kept            */
    seq_t a1 = base[1];
    seq_t a2 = base[2];
    seq_t a3 = base[3];
    seq_t a4 = base[4];
    seq_t a5 = base[5];
    seq_t a6 = base[6];
    seq_t cur = 0; /* will hold a(n) after the loop */

    for (idx_t k = 7; k <= n; ++k) {
        cur = 17 * a6 - 47 * a5 + 41 * a4 - 10 * a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = cur;
    }

    return cur;
}
