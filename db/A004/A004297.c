/*
 * A004297  Expansion of (1+2*x+x^2)/(1-58*x+x^2).
 * https://oeis.org/A004297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 60, a(2) = 3480, a(n) = 58*a(n-1) - a(n-2) for
 *            n >= 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)60;
    if (n == 2) return (seq_t)3480;
    
    seq_t prev2 = (seq_t)60;
    seq_t prev1 = (seq_t)3480;
    seq_t curr;
    
    for (idx_t i = 3; i <= n; i++) {
        curr = (seq_t)58 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
