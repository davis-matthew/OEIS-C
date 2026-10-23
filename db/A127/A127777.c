/*
 * A127777  A127773 * A002260 as infinite lower triangular matrices.
 * https://oeis.org/A127777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = c * (r * (r + 1) / 2) * where r = smallest integer with r
 *            * (r + 1) / 2 ≥ n * and c = n - (r - 1) * r / 2 (the column
 *            index in that row)
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A127777: row‑major enumeration of
 * the lower triangular matrix with entries T(n,k) = k * binomial(n+1,2). */
seq_t a(idx_t n)
{
    /* find the row r (1‑based) that contains the n‑th element */
    idx_t r = 1;
    /* Use a simple loop; for n up to ~10^12 this performs at most ~1.4 million
       iterations, which is well within limits for the expected use (n <= 10000). */
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* compute the column c (1‑based) within that row */
    idx_t prev = (r - 1) * r / 2;  /* number of elements before row r */
    idx_t c = n - prev;            /* column index within row r */

    /* triangular number for the row: r * (r + 1) / 2 */
    seq_t tri = ((seq_t)r * (seq_t)(r + 1)) / 2;

    /* final value: c times the row triangular number */
    return (seq_t)c * tri;
}
