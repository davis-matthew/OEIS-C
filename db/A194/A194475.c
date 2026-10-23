/*
 * A194475  Number of ways to arrange 3 indistinguishable points on an n X n X
 *          n triangular grid so that no three points are in the same row or
 *          diagonal.
 * https://oeis.org/A194475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 3*n^5 - 9*n^4 + n^3 + 8*n^2 - 4*n) / 48
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 42, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1, but return 0 */

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t n5 = n4 * N;
    seq_t n6 = n5 * N;

    /* Sum of positive terms */
    seq_t pos = n6 + ((seq_t)3 * n5) + n3 + ((seq_t)8 * n2);
    /* Sum of negative terms */
    seq_t neg = ((seq_t)9 * n4) + ((seq_t)4 * N);
    seq_t numer = pos - neg;            /* pos ≥ neg for all n ≥ 1 */
    return numer / ((seq_t)48);
}
