/*
 * A002418  4-dimensional figurate numbers: a(n) = (5*n-1)*binomial(n+2,3)/4.
 * https://oeis.org/A002418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n-1)*binomial(n+2,3)/4
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 274, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    // Compute binomial(n+2, 3) = (n+2)*(n+1)*n/6
    seq_t binom = ((n_seq + 2) * (n_seq + 1) / 2) * n_seq / 3;
    
    // Compute (5*n - 1)
    seq_t coeff = 5 * n_seq - 1;
    
    // Return (5*n-1)*binomial(n+2,3)/4
    return coeff * binom / 4;
}
