/*
 * A002533  a(n) = 2*a(n-1) + 5*a(n-2), with a(0) = a(1) = 1.
 * https://oeis.org/A002533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }
    
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)1;
    seq_t current;
    
    for (idx_t i = 2; i <= n; i++) {
        current = 2 * prev1 + 5 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    
    return current;
}
