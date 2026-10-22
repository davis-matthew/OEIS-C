/*
 * A003115  a(n) = 4^floor(n/2)*a(n-1) - a(n-2), for n >= 2, with a(0) = a(1)
 *          = 1.
 * https://oeis.org/A003115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^floor(n/2)*a(n-1) - a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    
    seq_t a_prev2 = 1;
    seq_t a_prev1 = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        idx_t exp = i / 2;
        seq_t pow_4 = (seq_t)1;
        seq_t base = (seq_t)4;
        
        while (exp > 0) {
            if (exp & 1) {
                pow_4 = pow_4 * base;
            }
            base = base * base;
            exp >>= 1;
        }
        
        seq_t a_curr = pow_4 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
