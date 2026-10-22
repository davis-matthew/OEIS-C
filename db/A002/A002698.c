/*
 * A002698  Coefficients of Chebyshev polynomials: n*(2*n-3)*2^(2*n-5).
 * https://oeis.org/A002698
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(2*n-3)*2^(2*n-5)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t product = (seq_t)n * (seq_t)(2 * n - 3);
    
    if (2 * n >= 5) {
        idx_t shift = 2 * n - 5;
        product = product << shift;
    } else {
        idx_t shift = 5 - 2 * n;
        product = product >> shift;
    }
    
    return product;
}
