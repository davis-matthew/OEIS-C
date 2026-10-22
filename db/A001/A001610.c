/*
 * A001610  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 0 and a(1) = 2.
 * https://oeis.org/A001610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, hear
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = 0, a(1) = 2
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 2;
    
    seq_t a_prev2 = 0, a_prev1 = 2;
    for (idx_t i = 2; i <= n; i++) {
        seq_t a_curr = a_prev1 + a_prev2 + 1;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
