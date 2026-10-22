/*
 * A000770  Stirling numbers of the second kind, S(n,6).
 * https://oeis.org/A000770
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = S2(n,6) via S2(m,k) = k*S2(m-1,k) + S2(m-1,k-1), S2(0,0)
 *            = 1, S2(0,k) = 0 for k > 0
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s[7];
    int k;
    idx_t m;
    s[0] = (seq_t)1;
    for (k = 1; k <= 6; k++) s[k] = (seq_t)0;
    for (m = 1; m <= n; m++) {
        for (k = 6; k >= 1; k--) {
            s[k] = (seq_t)k * s[k] + s[k - 1];
        }
        s[0] = (seq_t)0;
    }
    return s[6];
}
