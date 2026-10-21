/*
 * A228394  The number of permutations of length n sortable by 2 prefix block
 *          transpositions.
 * https://oeis.org/A228394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (n-1)*n*(3*n^2-11*n+16)/12
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, 0 is unused */
    seq_t N = (seq_t)n;
    seq_t term = (N - 1) * N *
                 ((seq_t)3 * N * N - (seq_t)11 * N + (seq_t)16) /
                 (seq_t)12 + (seq_t)1;
    return term;
}
