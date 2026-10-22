/*
 * A006587  a(n) = 3*2^(2*n)*(3*n)!/((2*n)!*n!).
 * https://oeis.org/A006587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(2*n)*(3*n)!/((2*n)!*n!) = 3*4^n*binomial(3*n,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* binomial(3n, n) built multiplicatively: after step i, c = binomial(2n+i, i) */
    seq_t c = 1;
    for (idx_t i = 1; i <= n; i++) {
        c = c * (seq_t)(2 * n + i) / (seq_t)i;
    }
    seq_t result = c * 3;
    for (idx_t i = 0; i < n; i++) {
        result *= 4;
    }
    return result;
}
