/*
 * A000771  Stirling numbers of second kind, S(n,7).
 * https://oeis.org/A000771
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = S2(n,7), via S2(m,k) = k*S2(m-1,k) + S2(m-1,k-1), S2(0,0)
 *            = 1, S2(0,k) = 0 for k > 0
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 16, u64 n <= 27, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s[8];
    idx_t m;
    int k;
    for (k = 0; k < 8; k++) s[k] = 0;
    s[0] = 1;
    for (m = 1; m <= n; m++) {
        for (k = 7; k >= 1; k--) {
            s[k] = s[k] * k + s[k - 1];
        }
        s[0] = 0;
    }
    return s[7];
}
