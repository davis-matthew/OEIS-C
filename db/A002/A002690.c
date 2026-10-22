/*
 * A002690  a(n) = (n+1) * (2*n)! / n!.
 * https://oeis.org/A002690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 4*n*a(n-1) + 4*(2*n-3)*a(n-2), a(0)=1, a(1)=4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    
    seq_t a_prev2 = (seq_t)1;
    seq_t a_prev1 = (seq_t)4;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t a_curr = (seq_t)4 * ni * a_prev1 + (seq_t)4 * ((seq_t)2 * ni - (seq_t)3) * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
