/*
 * A001340  E.g.f.: 2*exp(x)/(1-x)^3.
 * https://oeis.org/A001340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)*a(n-1) - (n-1)*a(n-2), a(0) = 2, a(1) = 8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)8;
    
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)8;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t coeff1 = (seq_t)i + (seq_t)3;
        seq_t coeff2 = (seq_t)i - (seq_t)1;
        curr = coeff1 * prev1 - coeff2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
