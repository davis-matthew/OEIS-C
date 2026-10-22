/*
 * A002203  Companion Pell numbers: a(n) = 2*a(n-1) + a(n-2), a(0) = a(1) = 2.
 * https://oeis.org/A002203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0) = 2, a(1) = 2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 2;
    
    seq_t prev2 = 2;
    seq_t prev1 = 2;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = 2 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
