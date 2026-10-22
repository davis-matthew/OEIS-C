/*
 * A002674  a(n) = (2n)!/2.
 * https://oeis.org/A002674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} (n^2 - k^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t n_cast = (seq_t)n;
    seq_t n_sq = n_cast * n_cast;
    
    for (idx_t k = 0; k < n; k++) {
        seq_t k_cast = (seq_t)k;
        seq_t k_sq = k_cast * k_cast;
        result *= (n_sq - k_sq);
    }
    
    return result;
}
