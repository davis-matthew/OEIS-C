/*
 * A003665  a(n) = 2^(n-1)*( 2^n + (-1)^n ).
 * https://oeis.org/A003665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    
    seq_t prev2 = 1, prev1 = 1, curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = 2 * prev1 + 8 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
