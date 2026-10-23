/*
 * A143254  Triangle read by rows, T(n,k) = (4n-3)*(4k-3); 1<=k<=n.
 * https://oeis.org/A143254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (4*R-3)*(4*C-3) * where R = smallest integer such that
 *            R*(R+1)/2 >= n * and C = n - R*(R-1)/2 (1‑based column index)
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find row R */
    idx_t R = 1;
    seq_t cum;
    while (1) {
        /* cum = R*(R+1)/2 in seq_t to avoid overflow of idx_t */
        cum = ((seq_t)R * (seq_t)(R + 1)) / 2;
        if (cum >= (seq_t)n)
            break;
        ++R;
    }

    /* Compute column C */
    seq_t prev = ((seq_t)(R - 1) * (seq_t)R) / 2;
    idx_t C = (idx_t)((seq_t)n - prev);   /* 1‑based */

    /* Compute the term */
    seq_t factor_row = (seq_t)4 * (seq_t)R - (seq_t)3;
    seq_t factor_col = (seq_t)4 * (seq_t)C - (seq_t)3;
    return factor_row * factor_col;
}
