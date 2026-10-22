/*
 * A003221  Number of even permutations of length n with no fixed points.
 * https://oeis.org/A003221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*(a(n-1)+a(n-2))+((-1)^(n-1))*(n-1) for n>1, a(0)=1,
 *            a(1)=0
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)0;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t n_minus_1 = (seq_t)(i - 1);
        seq_t term1 = n_minus_1 * (prev1 + prev2);
        seq_t curr;
        
        if (i % 2 == 0) {
            curr = term1 - n_minus_1;
        } else {
            curr = term1 + n_minus_1;
        }
        
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
