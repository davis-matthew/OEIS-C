/*
 * A001711  Generalized Stirling numbers, [n+4,4]_3.
 * https://oeis.org/A001711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)! * Sum_{k=0..n} (binomial(k+2,2)/(n+1-k))
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t fact = 1;
    
    /* Compute (n+1)! */
    for (idx_t i = 1; i <= n + 1; i++) {
        fact = fact * (seq_t)i;
    }
    
    /* Sum over k from 0 to n */
    for (idx_t k = 0; k <= n; k++) {
        /* Compute binomial(k+2, 2) = (k+2)*(k+1)/2 */
        seq_t binom = ((seq_t)(k + 2) * (seq_t)(k + 1)) / 2;
        
        /* Compute (n+1)! / (n+1-k) * binomial(k+2, 2) */
        seq_t term = (fact / (seq_t)(n + 1 - k)) * binom;
        result = result + term;
    }
    
    return result;
}
