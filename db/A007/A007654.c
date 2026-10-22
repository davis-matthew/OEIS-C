/*
 * A007654  Numbers k such that the standard deviation of 1,...,k is an
 *          integer.
 * https://oeis.org/A007654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 6, a(1) = 0, a(2) = 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 3;
    
    seq_t prev2 = 0;
    seq_t prev1 = 3;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t curr = 14 * prev1 - prev2 + 6;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
