/*
 * A005248  Bisection of Lucas numbers: a(n) = L(2*n) = A000032(2*n).
 * https://oeis.org/A005248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 2, a(1) = 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)3;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = (seq_t)3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
