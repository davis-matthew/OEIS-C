/*
 * A001848  Crystal ball sequence for 6-dimensional cubic lattice.
 * https://oeis.org/A001848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..min(6,n)} 2^k * binomial(6,k) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 59, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;
    
    static const unsigned c6[7] = {1, 6, 15, 20, 15, 6, 1};
    
    idx_t limit = (n < 6) ? n : 6;
    
    for (idx_t k = 0; k <= limit; k++) {
        /* Compute binomial(n, k) */
        seq_t binom_nk = 1;
        for (idx_t i = 0; i < k; i++) {
            binom_nk = binom_nk * (n_seq - (seq_t)i) / ((seq_t)i + 1);
        }
        
        /* Compute 2^k */
        seq_t pow2_k = 1;
        for (idx_t j = 0; j < k; j++) {
            pow2_k *= 2;
        }
        
        /* Add term: 2^k * C(6,k) * C(n,k) */
        result += pow2_k * (seq_t)c6[k] * binom_nk;
    }
    
    return result;
}
