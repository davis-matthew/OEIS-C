/*
 * A001077  Numerators of continued fraction convergents to sqrt(5).
 * https://oeis.org/A001077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, nice
 * Formula:   a(n) = 4*a(n-1) + a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)2;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = 4 * prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
