/*
 * A002315  NSW numbers: a(n) = 6*a(n-1) - a(n-2); also a(n)^2 - 2*b(n)^2 = -1
 *          with b(n) = A001653(n+1).
 * https://oeis.org/A002315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0) = 1, a(1) = 7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)7;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = (seq_t)6 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
