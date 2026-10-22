/*
 * A001849  Crystal ball sequence for 7-dimensional cubic lattice.
 * https://oeis.org/A001849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = D(7,n), Delannoy numbers: D(m,j) = D(m,j-1) + D(m-1,j) +
 *            D(m-1,j-1), D(0,j) = 1, D(m,0) = 1
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 39, u64 n <= 954, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t d[8];
    int m;
    idx_t j;
    for (m = 0; m < 8; m++)
        d[m] = 1;
    for (j = 1; j <= n; j++) {
        seq_t prev_old = d[0];
        d[0] = 1;
        for (m = 1; m < 8; m++) {
            seq_t old = d[m];
            d[m] = old + d[m - 1] + prev_old;
            prev_old = old;
        }
    }
    return d[7];
}
