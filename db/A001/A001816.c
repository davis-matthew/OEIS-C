/*
 * A001816  Coefficients of x^n in Hermite polynomial H_{n+4}.
 * https://oeis.org/A001816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12 * 2^n * C(n+4, 4) where C(n+4, 4) =
 *            (n+4)(n+3)(n+2)(n+1) / 24
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 16, u64 n <= 42, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    
    // Compute C(n+4, 4) = (n+4)(n+3)(n+2)(n+1) / 24
    seq_t binom = ((n_val + 1) * (n_val + 2) * (n_val + 3) * (n_val + 4)) / 24;
    
    // Start with 12 * C(n+4, 4)
    seq_t result = 12 * binom;
    
    // Multiply by 2^n
    for (idx_t i = 0; i < n; i++) {
        result *= 2;
    }
    
    return result;
}
