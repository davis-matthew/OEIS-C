/*
 * A007052  Number of order-consecutive partitions of n.
 * https://oeis.org/A007052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)3;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = (seq_t)4 * prev1 - (seq_t)2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
