/*
 * A007070  a(n) = 4*a(n-1) - 2*a(n-2) with a(0) = 1, a(1) = 4.
 * https://oeis.org/A007070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 4;
    
    seq_t prev2 = 1;
    seq_t prev1 = 4;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = 4 * prev1 - 2 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
