/*
 * A095729  A002260 squared, as an infinite lower triangular matrix, read by
 *          rows.
 * https://oeis.org/A095729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = j * (binomial(i+1,2) - binomial(j,2)) where binomial(k,2)
 *            = k*(k-1)/2. The index n is mapped to row i and column j by
 *            reading the lower‑triangular matrix row‑by‑row. The number of
 *            terms up to row r equals r(r+1)/2
 * Range:     u8 n <= 59, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest i such that i*(i+1)/2 >= n. */
static inline idx_t find_row(idx_t n)
{
    idx_t i = 1;
    while ((i * (i + 1)) / 2 < n) {
        ++i;
    }
    return i;
}

seq_t a(idx_t n)
{
    /* handle the offset: a(1) is the first term. */
    if (n == 0) return 0;

    /* Determine the row and column. */
    idx_t i = find_row(n);
    idx_t prev_terms = ((i - 1) * i) / 2;   /* terms before this row */
    idx_t j = n - prev_terms;                /* column index (1‑based) */

    /* Compute the two binomial terms. */
    seq_t S_i = ((seq_t)i * (seq_t)(i + 1)) / 2;   /* binomial(i+1,2) */
    seq_t S_j = ((seq_t)j * (seq_t)(j - 1)) / 2;   /* binomial(j,2) */

    /* Result. */
    return ((seq_t)j) * (S_i - S_j);
}
