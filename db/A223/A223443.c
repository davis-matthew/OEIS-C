/*
 * A223443  4-level binary fanout graph coloring a rectangular array: number
 *          of n X 2 0..14 arrays where 0..14 label nodes of a graph with
 *          edges 0,1 1,3 3,5 3,6 1,4 4,7 4,8 0,2 2,9 9,11 9,12 2,10 10,13
 *          10,14 and every array movement to a horizontal or vertical
 *          neighbor moves along an edge of this graph.
 * https://oeis.org/A223443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28 (n = 1); a(n) = 2^(n-2) * (4 + 25 * 2^n) for n > 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)28;
    }

    /* Exponents: e1 = n-2,   e2 = 2*n-2 */
    idx_t e1 = n - 2;
    idx_t e2 = (n << 1) - 2;  /* 2*n - 2, safe for n ≥ 2 */

    seq_t pow_n_2   = pow2(e1);    /* 2^(n-2) */
    seq_t pow_2n_2  = pow2(e2);    /* 2^(2n-2) */

    return (pow_n_2 * (seq_t)4) + (pow_2n_2 * (seq_t)25);
}
