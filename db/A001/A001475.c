/*
 * A001475  a(n) = a(n-1) + n * a(n-2), where a(1) = 1, a(2) = 2.
 * https://oeis.org/A001475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + n * a(n-2), a(1) = 1, a(2) = 2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 31, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 2;
    
    seq_t prev2 = 1;
    seq_t prev1 = 2;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t curr = prev1 + (seq_t)i * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
