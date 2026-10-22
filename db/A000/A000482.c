/*
 * A000482  Unsigned Stirling numbers of first kind s(n,5).
 * https://oeis.org/A000482
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = |s(n,5)|, computed by the triangle recurrence c(m+1,k) =
 *            m*c(m,k) + c(m,k-1), c(0,0) = 1, c(0,k) = 0 for k > 0
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c[6];
    int k;
    idx_t m;
    for (k = 0; k < 6; k++) c[k] = 0;
    c[0] = 1;
    for (m = 0; m < n; m++) {
        seq_t mv = (seq_t)m;
        for (k = 5; k >= 1; k--) {
            c[k] = mv * c[k] + c[k - 1];
        }
        c[0] = mv * c[0];
    }
    return c[5];
}
