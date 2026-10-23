/*
 * A155103  Triangle read by rows: Matrix inverse of A155102.
 * https://oeis.org/A155103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle read row‑wise; it equals *
 *            the (row, column) entry of the inverse of the lower‑triangular
 *            matrix * L with L_{i,i}=1 for all i and * L_{2m,m}=-(m+1) for
 *            m≥1; all other entries are 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the entry at position (r,c) (1‑based) in the inverse matrix.  */
static seq_t get_value(idx_t r, idx_t c)
{
    if (c > r) return 0;          /* out of range */
    if (r == 1) return 1;         /* first row/column is always 1 */

    /* Odd rows are – except for the diagonal – all zeros. */
    if ((r & 1u) == 1u)
        return (c == r) ? 1 : 0;

    /* Even r = 2m */
    if (c == r)          /* diagonal */
        return 1;

    idx_t m = r >> 1;    /* m = r/2 */
    if (c <= m) {
        seq_t val = get_value(m, c);
        seq_t factor = (seq_t)(m + 1);
        return val * factor;
    }
    return 0;             /* columns beyond m are zero for even rows */
}

seq_t a(idx_t n)
{
    /* Offset is 1; n≥1.  Find the row r that contains index n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (mid * (mid + 1) / 2 < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    /* Compute the column index within the found row. */
    idx_t offset = (r * (r - 1) / 2) + 1;
    idx_t c = n - offset + 1;  /* 1‑based column */

    return get_value(r, c);
}
