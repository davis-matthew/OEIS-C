/*
 * A001352  a(0) = 1, a(1) = 6, a(2) = 24; for n>=3, a(n) = 4a(n-1) - a(n-2).
 * https://oeis.org/A001352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 1, a(1) = 6, a(2) = 24
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 6;
    if (n == 2) return 24;
    
    seq_t prev2 = 6;
    seq_t prev1 = 24;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t next = 4 * prev1 - prev2;
        prev2 = prev1;
        prev1 = next;
    }
    
    return prev1;
}
