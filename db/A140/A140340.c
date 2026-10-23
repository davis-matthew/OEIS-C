/*
 * A140340  Square array read by rows: T(n,k) =
 *          19*n^2+10*k^2-(n-1)*(20*(k-1)+10), with 14 columns.
 * https://oeis.org/A140340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   a(n) = T(row,col) where row = floor((n-1)/14)+1, col =
 *            (n-1)%14+1, and T(n,k) = 19*n^2 + 10*k^2 - (n-1)*(20*(k-1)+10)
 * Range:     u8 n <= 4, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute row and column from row‑major indexing */
    idx_t row = (n - 1) / 14 + 1;
    idx_t col = (n - 1) % 14 + 1;

    /* Promote to seq_t for exact arithmetic */
    seq_t sr = (seq_t)row;
    seq_t sc = (seq_t)col;

    /* Apply the given quadratic formula */
    seq_t term = (seq_t)19 * sr * sr + (seq_t)10 * sc * sc
                 - (sr - (seq_t)1) * ((seq_t)20 * (sc - (seq_t)1) + (seq_t)10);
    return term;
}
