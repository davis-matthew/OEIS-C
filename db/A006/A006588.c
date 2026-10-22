/*
 * A006588  a(n) = 4^n*(3*n)!/((2*n)!*n!).
 * https://oeis.org/A006588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4^n*C(3*n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t power_4 = 1;
    for (idx_t i = 0; i < n; i++) {
        power_4 *= 4;
    }
    
    seq_t sn = (seq_t)n;
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        binom = binom * (3 * sn - si + 1) / si;
    }
    
    return power_4 * binom;
}
