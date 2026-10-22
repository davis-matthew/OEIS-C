/*
 * A001595  a(n) = a(n-1) + a(n-2) + 1, with a(0) = a(1) = 1.
 * https://oeis.org/A001595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = a(1) = 1
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    
    seq_t prev2 = 1;
    seq_t prev1 = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = prev1 + prev2 + 1;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
