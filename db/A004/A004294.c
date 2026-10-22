/*
 * A004294  Expansion of (1+2*x+x^2)/(1-34*x+x^2).
 * https://oeis.org/A004294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2), a(0) = 1, a(1) = 36, a(2) = 1224
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 36;
    if (n == 2) return 1224;
    
    seq_t a_prev_prev = 36;
    seq_t a_prev = 1224;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t a_curr = (seq_t)34 * a_prev - a_prev_prev;
        a_prev_prev = a_prev;
        a_prev = a_curr;
    }
    
    return a_prev;
}
