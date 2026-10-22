/*
 * A001863  Normalized total height of rooted trees with n nodes.
 * https://oeis.org/A001863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (n-2)! * Sum_{k=0..n-2} n^k/k!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;
    
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    seq_t pow_n = 1;  // n^k, starts with n^0
    
    for (idx_t k = 0; k < n - 1; k++) {
        // Compute (n-2)! / k! = (k+1) * (k+2) * ... * (n-2)
        seq_t prod = 1;
        for (idx_t j = k + 1; j < n - 1; j++) {
            prod *= (seq_t)j;
        }
        
        sum += pow_n * prod;
        pow_n *= n_seq;
    }
    
    return sum;
}
