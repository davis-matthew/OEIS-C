/*
 * A001091  a(n) = 8*a(n-1) - a(n-2); a(0) = 1, a(1) = 4.
 * https://oeis.org/A001091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
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
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = prev1 * (seq_t)8 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
