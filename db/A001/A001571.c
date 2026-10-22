/*
 * A001571  a(n) = 4*a(n-1) - a(n-2) + 1, with a(0) = 0, a(1) = 2.
 * https://oeis.org/A001571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 1, a(0) = 0, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 2;
    
    seq_t prev2 = 0;
    seq_t prev1 = 2;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = 4 * prev1 - prev2 + 1;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
