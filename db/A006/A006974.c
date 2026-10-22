/*
 * A006974  Coefficients of Chebyshev T polynomials: a(n) = A053120(n+8, n), n
 *          >= 0.
 * https://oeis.org/A006974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) * binomial(n+3, 3) * (n+8)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    // Compute C(n+3, 3) = (n+3)(n+2)(n+1)/6
    seq_t binom = (n_seq + 3) * (n_seq + 2) * (n_seq + 1) / 6;
    
    // Compute the main product: binom * (n+8)
    seq_t result = binom * (n_seq + 8);
    
    // Apply the power of 2
    if (n >= 3) {
        result = result << (n - 3);
    } else {
        result = result >> (3 - n);
    }
    
    return result;
}
