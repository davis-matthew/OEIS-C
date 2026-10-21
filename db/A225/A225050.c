/*
 * A225050  Number of shortest paths from one vertex of a cube (side = n
 *          units) to farthest vertex, along the grid on 3 surfaces meeting at
 *          another vertex.
 * https://oeis.org/A225050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(3n,n) - binomial(2n,n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t c3n_n = binomial((idx_t)(3 * n), n); /* binomial(3n, n) */
    seq_t c2n_n = binomial((idx_t)(2 * n), n); /* binomial(2n, n) */
    seq_t result = c3n_n * (seq_t)2 - c2n_n;
    return result;
}
