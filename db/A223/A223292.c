/*
 * A223292  4-loop graph coloring a rectangular array: number of n X 3 0..8
 *          arrays where 0..8 label nodes of a graph with edges 0,1 1,2 2,0
 *          0,3 3,4 4,0 0,5 5,6 6,0 0,7 7,8 8,0 and every array movement to a
 *          horizontal or vertical neighbor moves along an edge of this graph.
 * https://oeis.org/A223292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 531*a(n-2) - 1022*a(n-3) - 750*a(n-4) +
 *            448*a(n-5) with initial values: a(1) = 96 a(2) = 1368 a(3) =
 *            42168 a(4) = 709320 a(5) = 22346136 The function computes the
 *            recurrence directly, using mul_small to avoid overflow on
 *            arbitrary‑precision builds
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Base terms (offset 1) */
    static const seq_t init[5] = {
        (seq_t)96,
        (seq_t)1368,
        (seq_t)42168,
        (seq_t)709320,
        (seq_t)22346136
    };

    if (n <= 5)
        return init[n - 1];

    /* a1..a5 correspond to a(n-5) .. a(n-1) for the current iteration */
    seq_t a1 = init[0];
    seq_t a2 = init[1];
    seq_t a3 = init[2];
    seq_t a4 = init[3];
    seq_t a5 = init[4];

    for (idx_t i = 6; i <= n; ++i) {
        seq_t cur = mul_small(a5, 2);          /* 2*a(n-1) */
        cur += mul_small(a4, 531);            /* + 531*a(n-2) */
        cur -= mul_small(a3, 1022);           /* - 1022*a(n-3) */
        cur -= mul_small(a2, 750);            /* - 750*a(n-4) */
        cur += mul_small(a1, 448);            /* + 448*a(n-5) */

        /* Shift window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = cur;
    }

    return a5;
}
