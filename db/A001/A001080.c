/*
 * A001080  a(n) = 16*a(n-1) - a(n-2) with a(0) = 0, a(1) = 3.
 * https://oeis.org/A001080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - a(n-2), a(0) = 0, a(1) = 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 3;
    
    seq_t prev2 = 0;
    seq_t prev1 = 3;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = 16 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
