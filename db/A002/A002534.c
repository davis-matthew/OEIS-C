/*
 * A002534  a(n) = 2*a(n-1) + 9*a(n-2), with a(0) = 0, a(1) = 1.
 * https://oeis.org/A002534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 9*a(n-2), a(0) = 0, a(1) = 1 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    
    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)1;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = 2 * prev1 + 9 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
