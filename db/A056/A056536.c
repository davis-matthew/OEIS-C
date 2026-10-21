/*
 * A056536  Mapping from half-antidiagonal reading of the triangle (as used in
 *          A028297) to the column-by-column reading of the triangular tables.
 * https://oeis.org/A056536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((r-1)*r)/2 + 1 - [ (c-1)*(r-2) - (c-2)*(c-1)/2 ] Here r
 *            is the row index of n in the triangular array and c = n - N(r-1)
 *            is the position within that row. N(r) is the cumulative number
 *            of elements up to row r: N(r) = floor((r+1)/2) + floor((r+2)/2)
 *            + ... + floor((2r)/2) = { m^2 if r = 2m-1...
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No additional headers are required besides the build's definitions
   of seq_t and idx_t, which are unsigned integer types. */

static inline idx_t cumulative(idx_t r) {
    if (r == 0) return 0;
    if (r & 1) {                        /* r odd: r = 2m-1 */
        idx_t m = (r + 1) >> 1;          /* m = (r+1)/2 */
        return m * m;
    } else {                            /* r even: r = 2m   */
        idx_t m = r >> 1;
        return m * (m + 1);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset 1 in OEIS, n>=1 valid */

    /* Find the row r such that N(r-1) < n <= N(r) */
    idx_t r = 1;
    while (cumulative(r) < n) r++;

    /* Position within the row */
    idx_t c = n - cumulative(r - 1);
    idx_t s = c - 1;                    /* s = c-1 */

    /* Base value of the first element in row r */
    seq_t base = ((seq_t)(r - 1) * (seq_t)r) / 2 + 1;

    if (s == 0) return base;            /* first element of the row */

    /* Subtract the cumulative differences */
    seq_t sub = ((seq_t)s * (seq_t)(r - 2));
    if (s >= 2) {
        sub -= ((seq_t)(s - 1) * (seq_t)s) / 2;
    }

    return base - sub;
}
