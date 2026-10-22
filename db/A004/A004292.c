/*
 * A004292  Expansion of (1+x)^2/(1-18*x+x^2).
 * https://oeis.org/A004292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2) for n >= 3, a(0) = 1, a(1) = 20, a(2)
 *            = 360
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 20;
    if (n == 2) return 360;
    
    seq_t prev2 = 20;
    seq_t prev1 = 360;
    seq_t curr;
    
    for (idx_t i = 3; i <= n; i++) {
        curr = 18 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
