/*
 * A001921  a(n) = 14*a(n-1) - a(n-2) + 6 for n>1, a(0)=0, a(1)=7.
 * https://oeis.org/A001921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 6, a(0) = 0, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)7;
    
    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)7;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = (seq_t)14 * prev1 - prev2 + (seq_t)6;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
