/*
 * A001644  a(n) = a(n-1) + a(n-2) + a(n-3), a(0)=3, a(1)=1, a(2)=3.
 * https://oeis.org/A001644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3), a(0)=3, a(1)=1, a(2)=3
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    
    seq_t prev0 = (seq_t)3;
    seq_t prev1 = (seq_t)1;
    seq_t prev2 = (seq_t)3;
    seq_t current;
    
    for (idx_t i = 3; i <= n; i++) {
        current = prev0 + prev1 + prev2;
        prev0 = prev1;
        prev1 = prev2;
        prev2 = current;
    }
    
    return current;
}
