/*
 * A004299  Expansion of (1+2*x+x^2)/(1-74*x+x^2).
 * https://oeis.org/A004299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 74*a(n-1) - a(n-2), a(0) = 1, a(1) = 76, a(2) = 5624
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)76;
    if (n == 2) return (seq_t)5624;
    
    seq_t prev2 = (seq_t)76;
    seq_t prev1 = (seq_t)5624;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t curr = (seq_t)74 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
