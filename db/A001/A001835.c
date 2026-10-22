/*
 * A001835  a(n) = 4*a(n-1) - a(n-2), with a(0) = 1, a(1) = 1.
 * https://oeis.org/A001835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    seq_t prev2 = 1;
    seq_t prev1 = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = 4 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
