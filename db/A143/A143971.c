/*
 * A143971  Triangle read by rows, (3n-2) subsequences decrescendo.
 * https://oeis.org/A143971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) with T(r,c)=3*r-3*c+1; * row = smallest r
 *            such that r(r+1)/2 >= n; * col = n - (row-1)*row/2
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset starts at 1; 0 is undefined */

    idx_t r = 1;                 /* current row number */
    idx_t cumulative = 1;        /* total elements up to current row */

    /* Find the row containing index n */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }

    /* Elements before this row */
    idx_t prev = cumulative - r; /* sum of first (r-1) rows */
    idx_t col = n - prev;        /* position within row r (1‑based) */

    /* Compute the triangle entry */
    seq_t val = (seq_t)(3 * (seq_t)r - 3 * (seq_t)col + 1);
    return val;
}
