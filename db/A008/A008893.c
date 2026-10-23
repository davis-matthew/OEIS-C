/*
 * A008893  Number of equilateral triangles formed by triples of points taken
 *          from a hexagonal chunk of side n in the hexagonal lattice.
 * https://oeis.org/A008893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n^2+7*n+2)/4
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 221, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure we work in seq_t for every intermediate value */
    seq_t n_seq   = (seq_t)n;
    seq_t n_plus1 = n_seq + 1;
    seq_t term2   = (7 * n_seq * n_seq) + (7 * n_seq) + 2;

    seq_t prod = n_seq * n_plus1;
    prod = prod * term2;
    prod = prod / 4;          /* division is exact */

    return prod;
}
