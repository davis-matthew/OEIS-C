/*
 * A001762  Number of labeled n-vertex dissections of a ball.
 * https://oeis.org/A001762
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n,3) * (3*n-9)! / (2*n-4)!, n >= 4; a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 3) return 1;
    if (n == 4) return 1;
    
    // For n >= 5: a(n) = C(n,3) * product_{i=(2n-3)}^{(3n-9)} i
    seq_t binom = (seq_t)n * (seq_t)(n-1) * (seq_t)(n-2) / 6;
    
    seq_t prod = 1;
    for (idx_t i = 2*n - 3; i <= 3*n - 9; i++) {
        prod *= (seq_t)i;
    }
    
    return binom * prod;
}
