/*
 * A001708  Generalized Stirling numbers, [n+6,6]_2.
 * https://oeis.org/A001708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c_4(n+4), where c_j(m+1) = (m+2)*c_j(m) + c_{j-1}(m),
 *            c_0(m) = (m+1)!, c_j(0) = 0 for j > 0, c_0(0) = 1 (from (1-x)
 *            f_j' = 2 f_j + f_{j-1}, with f_j = (-log(1-x))^j / (j! (1-x)^2))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c[5];
    int j;
    idx_t m;
    c[0] = 1;
    for (j = 1; j < 5; j++) c[j] = 0;
    for (m = 0; m < n + 4; m++) {
        seq_t f = (seq_t)m + 2;
        for (j = 4; j >= 1; j--) {
            c[j] = f * c[j] + c[j - 1];
        }
        c[0] = f * c[0];
    }
    return c[4];
}
