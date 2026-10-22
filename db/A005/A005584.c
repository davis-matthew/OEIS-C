/*
 * A005584  Coefficients of Chebyshev polynomials.
 * https://oeis.org/A005584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5, n-1) + binomial(n+4, n-1)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 116, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t) n;
    
    /* a(n) = C(n+5, 6) + C(n+4, 5)
     * = n*(n+1)*(n+2)*(n+3)*(n+4)*(n+5)/720 + n*(n+1)*(n+2)*(n+3)*(n+4)/120
     * = n*(n+1)*(n+2)*(n+3)*(n+4) * (n+11) / 720
     * Rewrite to avoid overflow: [n*(n+1)*(n+2)*(n+3)*(n+4)/120] * (n+11) / 6 */
    
    seq_t p = n_seq * (n_seq + 1) * (n_seq + 2) * (n_seq + 3) * (n_seq + 4) / 120;
    return p * (n_seq + 11) / 6;
}
