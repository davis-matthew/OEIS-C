/*
 * A005668  Denominators of continued fraction convergents to sqrt(10).
 * https://oeis.org/A005668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    seq_t prev2 = 0;
    seq_t prev1 = 1;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = 6 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
