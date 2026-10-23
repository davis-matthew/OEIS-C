/*
 * A030308  Triangle T(n, k): Write n in base 2, reverse order of digits, to
 *          get the n-th row.
 * https://oeis.org/A030308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   a(n) = (m >> k) & 1 where m is the smallest integer with
 *            cumulative row length > n and k = n - cumulative length of rows
 *            < m. This selects the bit at position k in the reversed binary
 *            representation of m
 * Range:     u8 n <= 7465, u16 n <= 9217, u32 n <= 9217, u64 n <= 9217,
 *            u128 n <= 9217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t bitlen(idx_t x) {
    idx_t len = 0;
    while (x) { x >>= 1; ++len; }
    return len ? len : 1;      /* for x==0 the row length is defined as 1 */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* row 0, element 0 */

    idx_t cum = 1;   /* cumulative count up to row 0 (length 1) */
    idx_t m = 0;     /* current row index */

    while (1) {
        ++m;                     /* next row */
        idx_t len = bitlen(m);   /* length of this row */

        /* If adding this row exceeds n, m is the desired row */
        if (cum + len > n) break;

        cum += len;              /* include this row */
    }

    idx_t k = n - cum;                /* position within row m */
    return ((seq_t)m >> k) & (seq_t)1; /* requested bit */
}
