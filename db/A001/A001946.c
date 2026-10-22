/*
 * A001946  a(n) = 11*a(n-1) + a(n-2).
 * https://oeis.org/A001946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*a(n-1) + a(n-2), a(0) = 2, a(1) = 11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)11;
    
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)11;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = (seq_t)11 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
