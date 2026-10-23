/*
 * A180569  The Wiener index of the P_3 X P_n grid, where P_m is the path
 *          graph on m nodes. The Wiener index of a connected graph is the sum
 *          of distances between all unordered pairs of nodes in the graph.
 * https://oeis.org/A180569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+3)*(3*n-1)/2
 * Range:     u8 n <= 4, u16 n <= 34, u32 n <= 1419, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* defined only for n ≥ 1, but safe guard */
    /* Convert the index to the result type to avoid narrowing */
    seq_t N = (seq_t)n;                   /* n as seq_t */
    seq_t term = N * (N + 3);             /* n*(n+3)  */
    term /= 2;                            /* divide by 2 – always even */
    idx_t factor = 3 * n - 1;             /* (3*n-1) computed in idx_t */
    term *= (seq_t)factor;                /* multiply by (3*n-1) */
    return term;
}
