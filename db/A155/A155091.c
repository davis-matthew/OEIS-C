/*
 * A155091  Triangle read by rows. Signed version of A145362. Main diagonal
 *          positive, rest of the nonzero terms negative.
 * https://oeis.org/A155091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = 1 if n == r(r+1)/2 (diagonal of the triangle); a(n) = 0
 *            if n <= r(r+1)/2 - floor((r-1)/2) - 1 (leading zeros of row r);
 *            a(n) = -1 otherwise. Here r is the unique integer with r(r+1)/2
 *            >= n
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 – defensive case */

    /* Find the row r by accumulating triangular numbers. */
    idx_t r = 1;
    __int128 t = 1;                /* t = r*(r+1)/2  for the current r */
    while (t < n) {
        ++r;
        t += r;                    /* t now becomes the triangular number of this r */
    }

    /* t now equals the triangular number of row r.
       prev = t - r  are the indices before this row. */
    idx_t prev = (idx_t)(t - r);
    idx_t c = n - prev;            /* column within the row (1‑based) */

    idx_t z = (r - 1) / 2;          /* number of leading zeros in this row */

    if (c <= z) return 0;          /* leading zeros */
    if (c == r) return 1;          /* diagonal entry (main diagonal) */
    return -1;                     /* remaining non‑zero entries are -1 */
}
