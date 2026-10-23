/*
 * A156319  Triangle by columns: (1, 2, 0, 0, 0, ...) in every column.
 * https://oeis.org/A156319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is produced by reading the triangle defined by T(r,c) = 1
 *            if r = c, 2 if c = r-1, and 0 otherwise, row by row. This
 *            function maps the column–row index n to that triangle entry
 * Range:     u8 n <= 44, u16 n <= 5049, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* binary search for the row r such that t_{r-1} < n <= t_r
       where t_r = r*(r+1)/2. */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t t_mid = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (t_mid >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;
    seq_t t_prev = (seq_t)(r - 1) * ((seq_t)(r - 1) + 1) / 2; /* t_{r-1} */
    idx_t c = (idx_t)(((seq_t)n - t_prev));                 /* column position (1‑based) */

    if (c == r)
        return (seq_t)1;
    if (c == r - 1)
        return (seq_t)2;
    return (seq_t)0;
}
