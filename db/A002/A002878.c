/*
 * A002878  Bisection of Lucas sequence: a(n) = L(2*n+1).
 * https://oeis.org/A002878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = 3*a(n) - a(n-1), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 4;
    
    seq_t prev2 = 1;
    seq_t prev1 = 4;
    seq_t curr;
    
    for (idx_t i = 2; i <= n; i++) {
        curr = 3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
