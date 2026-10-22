/*
 * A001541  a(0) = 1, a(1) = 3; for n > 1, a(n) = 6*a(n-1) - a(n-2).
 * https://oeis.org/A001541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)3;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = (seq_t)6 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
