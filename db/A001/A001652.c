/*
 * A001652  a(n) = 6*a(n-1) - a(n-2) + 2 with a(0) = 0, a(1) = 3.
 * https://oeis.org/A001652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 6*a(n-1) - a(n-2) + 2, with a(0) = 0, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    
    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)3;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = 6 * prev1 - prev2 + (seq_t)2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
