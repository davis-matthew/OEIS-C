/*
 * A134574  Array, a(n,k) = total size of all n-length words on a k-letter
 *          alphabet, read by antidiagonals.
 * https://oeis.org/A134574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n,k) = n*k^n. The function returns the n‑th entry of the
 *            sequence obtained by reading the table a(n,k) by antidiagonals,
 *            i.e. for each diagonal d ≥ 2 list the terms (n = d‑s, k = s)
 *            with s = 1…d‑1
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t m)
{
    if (m == 0)           /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* Find diagonal d such that cum(d‑1) < m ≤ cum(d), where
       cum(d) = d*(d‑1)/2.  Use a linear search; d grows like √(2m).  */
    idx_t d = 2;
    idx_t cum = 1;            /* cum(2) = 1 */
    while (cum < m) {
        ++d;
        cum += d - 1;         /* cum(d) = cum(d‑1) + (d‑1) */
    }
    idx_t prev_cum = cum - (d - 1);  /* cum(d‑1) */
    idx_t s = m - prev_cum;          /* position inside the diagonal */
    idx_t n_row = d - s;             /* row index i */
    idx_t k_col = s;                 /* column index j */

    /* Compute k_col ^ n_row */
    seq_t term = (seq_t)1;
    seq_t base = (seq_t)k_col;
    for (idx_t e = 0; e < n_row; ++e)
        term = term * base;

    /* Multiply by row index n_row */
    return term * (seq_t)n_row;
}
