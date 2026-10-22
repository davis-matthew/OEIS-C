/*
 * A006107  Gaussian binomial coefficient [ n,4 ] for q = 4.
 * https://oeis.org/A006107
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   q-Pascal recurrence [m,k] = [m-1,k-1] + 4^k*[m-1,k], [0,0] = 1,
 *            [0,k] = 0 for k > 0; a(n) = [n,4]
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c[5];
    seq_t qk[5];
    idx_t m;
    int k;
    c[0] = 1;
    for (k = 1; k <= 4; k++) c[k] = 0;
    qk[0] = 1;
    for (k = 1; k <= 4; k++) qk[k] = qk[k - 1] * 4;
    for (m = 1; m <= n; m++) {
        for (k = 4; k >= 1; k--) {
            c[k] = c[k - 1] + qk[k] * c[k];
        }
    }
    return c[4];
}
