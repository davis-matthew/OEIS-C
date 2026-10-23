/*
 * A118413  Triangle read by rows: T(n,k) = (2*n-1)*2^(k-1), 0<k<=n.
 * https://oeis.org/A118413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (2*row-1) * 2^(col-1) where row and col are obtained from
 *            the triangular indexing that unfolds the triangle reading rows
 *            left‑to‑right
 * Range:     u8 n <= 20, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) {
        ++r;
    }
    /* Total terms before this row */
    unsigned __int128 prev_total128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev_total = (idx_t)prev_total128;           /* fits in idx_t */
    /* Column index in this row (1‑based) */
    idx_t col = n - prev_total;
    /* Compute 2^(col-1) safely for all bit widths */
    seq_t power = 1;
    for (idx_t i = 1; i < col; ++i) {
        power *= 2;
    }
    /* Compute final value */
    seq_t result = (seq_t)(2 * r - 1) * power;
    return result;
}
