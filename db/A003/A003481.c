/*
 * A003481  a(n) = 7*a(n-1) - a(n-2) + 5.
 * https://oeis.org/A003481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - a(n-2) + 5, with a(0) = 2, a(1) = 20
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)20;
    
    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)20;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t curr = (seq_t)7 * prev1 - prev2 + (seq_t)5;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
