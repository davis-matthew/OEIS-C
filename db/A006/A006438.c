/*
 * A006438  Expansion of e.g.f. 1/sqrt(1-8x+x^2).
 * https://oeis.org/A006438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+2) = 4*(2*n+3)*a(n+1) - (n+1)^2*a(n), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 4;
    
    seq_t a_prev2 = 1;
    seq_t a_prev1 = 4;
    seq_t a_curr;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t i_s = (seq_t)i;
        a_curr = 4 * (2 * i_s - 1) * a_prev1 - (i_s - 1) * (i_s - 1) * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_curr;
}
