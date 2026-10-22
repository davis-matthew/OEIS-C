/*
 * A001724  Generalized Stirling numbers, [n+9,9]_5.
 * https://oeis.org/A001724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*(n+6)*a(n-1) - 5*(2*n²+22*n+61)*a(n-2) + 5*(n+5)*(2*n²+20*n+51)*a(n-3)
               - (5*n⁴+90*n³+610*n²+1845*n+2101)*a(n-4) + (n+4)⁵*a(n-5)
   Base cases: a(0)=1, a(1)=35, a(2)=835, a(3)=17360, a(4)=342769 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 35;
    if (n == 2) return 835;
    if (n == 3) return 17360;
    if (n == 4) return 342769;
    
    seq_t a_prev5 = 1;
    seq_t a_prev4 = 35;
    seq_t a_prev3 = 835;
    seq_t a_prev2 = 17360;
    seq_t a_prev1 = 342769;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t n_val = (seq_t)i;
        seq_t n2 = n_val * n_val;
        seq_t n3 = n2 * n_val;
        seq_t n4 = n3 * n_val;
        
        seq_t coef1 = 5 * (n_val + 6);
        seq_t coef2 = 5 * (2*n2 + 22*n_val + 61);
        seq_t coef3 = 5 * (n_val + 5) * (2*n2 + 20*n_val + 51);
        seq_t coef4 = 5*n4 + 90*n3 + 610*n2 + 1845*n_val + 2101;
        seq_t coef5 = (n_val + 4) * (n_val + 4) * (n_val + 4) * (n_val + 4) * (n_val + 4);
        
        seq_t a_curr = coef1 * a_prev1 - coef2 * a_prev2 + coef3 * a_prev3 - coef4 * a_prev4 + coef5 * a_prev5;
        
        a_prev5 = a_prev4;
        a_prev4 = a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
