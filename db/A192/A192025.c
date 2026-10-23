/*
 * A192025  The Wiener index of the double-comb graph \/_\/_\/...\/_\/ with 3n
 *          (n>=1) nodes. The Wiener index of a connected graph is the sum of
 *          the distances between all unordered pairs of vertices in the
 *          graph.
 * https://oeis.org/A192025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(3*n^2+12*n-7)/2
 * Range:     u8 n <= 3, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast index to seq_t to avoid overflow of intermediate integer operations */
    seq_t n_s = (seq_t)n;

    /* Compute 3*n^2 and 12*n as seq_t */
    seq_t term1 = 3 * n_s * n_s;   /* 3*n^2 */
    seq_t term2 = 12 * n_s;        /* 12*n */

    /* Combine terms: (3*n^2 + 12*n - 7) */
    seq_t temp = term1 + term2 - (seq_t)7;

    /* Multiply by n and divide by 2 */
    temp = temp * n_s;
    temp = temp / (seq_t)2;

    return temp;
}
