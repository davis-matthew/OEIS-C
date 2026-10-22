/*
 * A001653  Numbers k such that 2*k^2 - 1 is a square.
 * https://oeis.org/A001653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1) = 1, a(2) = 5
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 5;
    
    seq_t prev2 = 1;
    seq_t prev1 = 5;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t curr = 6 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
