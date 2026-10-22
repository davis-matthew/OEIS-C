/*
 * A002544  a(n) = binomial(2*n+1,n)*(n+1)^2.
 * https://oeis.org/A002544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(2*n+1, n) * (n+1)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_plus_1 = n_seq + 1;
    
    // Compute binomial(2n+1, n) iteratively: product_{i=1}^{n} (n+i+1) / i
    seq_t binom = 1;
    for (idx_t i = 1; i <= n; i++) {
        binom = binom * (n_seq + (seq_t)i + 1) / (seq_t)i;
    }
    
    // Return (n+1)^2 * binomial(2n+1, n)
    return binom * n_plus_1 * n_plus_1;
}
