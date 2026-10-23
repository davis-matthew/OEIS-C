/*
 * A153187  Triangle sequence: T(n, k) = -Product_{j=0..k+1} ((n+1)*j - 1).
 * https://oeis.org/A153187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) with r = floor((√(8n+1)-1)/2) and k = n -
 *            r(r+1)/2, where T(r, k) = -∏_{j=0}^{k+1} ((r+1)·j - 1). Since
 *            the term j=0 is –1, the product can be computed as a(n) =
 *            ∏_{j=1}^{k+1} ((r+1)·j - 1)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 33, u64 n <= 75, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term (0‑based) of OEIS A153187. */
seq_t a(idx_t n)
{
    /* Locate the row r containing index n.  Rows are
       offset_row(r) = r*(r+1)/2.  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column k in that row.  */
    idx_t col = n - (r * (r + 1)) / 2;

    /* Compute the product ∏_{j=1}^{col+1} ((r+1)·j - 1).  */
    seq_t value = 1;
    for (idx_t j = 1; j <= col + 1; ++j)
    {
        seq_t term = (seq_t)(r + 1) * (seq_t)j - 1;
        value *= term;
    }
    return value;
}
