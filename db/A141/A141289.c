/*
 * A141289  Triangle read by rows, n-th row = (n-2)-th row appended to the
 *          beginning of (n-1)-th row, + n.
 * https://oeis.org/A141289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Triangle read by rows, n‑th row = (n‑2)-th row appended to the
 *            beginning of (n‑1)-th row, + n
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t row_value(idx_t r, idx_t p, const idx_t *len) {
    if (r == 1) return (seq_t)1;
    if (r == 2) return (seq_t)(p == 0 ? 1 : 2);
    idx_t len2 = len[r-2];
    idx_t len1 = len[r-1];
    if (p < len2)          return row_value(r-2, p,     len);
    if (p < len2 + len1)   return row_value(r-1, p-len2, len);
    return (seq_t)r;          /* last element of the row */
}

seq_t a(idx_t n) {
    /* table of row lengths, indexed by 1… */
    static idx_t len[2000];
    /* current number of rows and cumulative term count */
    static idx_t rows = 0;
    static idx_t cumulative = 0;
    /* initialise first two rows once */
    if (rows == 0) {
        len[1] = 1;
        len[2] = 2;
        cumulative = 3;        /* len[1]+len[2] */
        rows = 2;
    }
    /* grow table until it covers the requested index n */
    while (cumulative < n) {
        ++rows;
        len[rows] = len[rows-2] + len[rows-1] + 1;
        cumulative += len[rows];
    }
    /* find the row containing n and the zero‑based position within that row */
    idx_t cum = 0;
    idx_t r = 0;
    idx_t pos = 0;
    for (r = 1; r <= rows; ++r) {
        if (n <= cum + len[r]) {
            pos = n - cum - 1;     /* zero‑based index */
            break;
        }
        cum += len[r];
    }
    return row_value(r, pos, len);
}
