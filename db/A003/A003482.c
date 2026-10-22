/*
 * A003482  a(n) = 7*a(n-1) - a(n-2) + 4, with a(0) = 0, a(1) = 5.
 * https://oeis.org/A003482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - a(n-2) + 4, a(0) = 0, a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)5;
    
    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)5;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = (seq_t)7 * prev1 - prev2 + (seq_t)4;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
