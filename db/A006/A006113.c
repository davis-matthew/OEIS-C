/*
 * A006113  Gaussian binomial coefficient [ n,4 ] for q = 5.
 * https://oeis.org/A006113
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n,4]_5 via the q-Pascal recurrence [m,k] = [m-1,k-1] +
 *            5^k*[m-1,k], [m,0] = 1, [0,k] = 0 for k > 0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c[5];
    seq_t pk[5];
    idx_t m;
    int k;
    for (k = 0; k < 5; k++) c[k] = 0;
    c[0] = 1;
    pk[0] = 1;
    for (k = 1; k < 5; k++) pk[k] = pk[k - 1] * 5;
    for (m = 1; m <= n; m++) {
        int top = m < 4 ? (int)m : 4;
        for (k = top; k >= 1; k--) {
            c[k] = c[k - 1] + pk[k] * c[k];
        }
    }
    return c[4];
}
