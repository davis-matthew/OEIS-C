/*
 * A000435  Normalized total height of all nodes in all rooted trees with n
 *          labeled nodes.
 * https://oeis.org/A000435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)! * Sum_{k=0..n-2} n^k/k!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;
    
    // Compute (n-1)!
    seq_t term_factor = 1;
    for (idx_t i = 1; i < n; i++) {
        term_factor *= (seq_t)i;
    }
    
    seq_t n_pow = 1;  // n^0
    
    for (idx_t k = 0; k <= n - 2; k++) {
        result += term_factor * n_pow;
        n_pow *= n_seq;
        term_factor /= (seq_t)(k + 1);
    }
    
    return result;
}
