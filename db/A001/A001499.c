/*
 * A001499  Number of n X n matrices with exactly 2 1's in each row and
 *          column, other entries 0.
 * https://oeis.org/A001499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   2*a(n) = 2*n*(n-1)*a(n-1) + n*(n-1)^2*a(n-2), a(0)=1, a(1)=0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 0;
    
    seq_t a_prev2 = 1;
    seq_t a_prev1 = 0;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t i_val = (seq_t)i;
        seq_t i_minus_1 = i_val - 1;
        seq_t coeff1 = i_val * i_minus_1;
        seq_t coeff2 = (i_val * i_minus_1 * i_minus_1) / 2;
        
        seq_t a_curr = coeff1 * a_prev1 + coeff2 * a_prev2;
        
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
