/*
 * A005429  Apéry numbers: n^3*C(2n,n).
 * https://oeis.org/A005429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n^3 * C(2n, n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    
    seq_t n_seq = (seq_t)n;
    
    // Compute C(2n, n) multiplicatively to avoid overflow
    seq_t binom_coeff = 1;
    for (idx_t i = 1; i <= n; i++) {
        binom_coeff = binom_coeff * (n_seq + (seq_t)i) / (seq_t)i;
    }
    
    // Compute n^3 * C(2n, n)
    seq_t n_cubed = n_seq * n_seq * n_seq;
    return n_cubed * binom_coeff;
}
