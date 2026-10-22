/*
 * A001570  Numbers k such that k^2 is centered hexagonal.
 * https://oeis.org/A001570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 14*a(n-1) - a(n-2), a(1) = 1, a(2) = 13
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)13;
    
    seq_t prev2 = (seq_t)1;   // a(1)
    seq_t prev1 = (seq_t)13;  // a(2)
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t curr = (seq_t)14 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
