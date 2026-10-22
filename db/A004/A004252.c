/*
 * A004252  Path sums of n-point graphs.
 * https://oeis.org/A004252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, nice
 * Formula:   a(n) = 8*a(n-1) - 21*a(n-2) + 19*a(n-3), with a(1) = 1, a(2) =
 *            2, a(3) = 4
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    
    seq_t a_prev3 = (seq_t)1;
    seq_t a_prev2 = (seq_t)2;
    seq_t a_prev1 = (seq_t)4;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t a_curr = (seq_t)8 * a_prev1 - (seq_t)21 * a_prev2 + (seq_t)19 * a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
