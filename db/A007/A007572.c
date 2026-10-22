/*
 * A007572  Generalization of the golden ratio (expansion of
 *          (5-13x)/((1+x)*(1-4x))).
 * https://oeis.org/A007572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 4*a(n-2), a(0) = 5, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)2;
    
    seq_t prev2 = (seq_t)5;
    seq_t prev1 = (seq_t)2;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = (seq_t)3 * prev1 + (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
