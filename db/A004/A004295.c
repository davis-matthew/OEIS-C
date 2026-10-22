/*
 * A004295  Expansion of (1+2*x+x^2)/(1-42*x+x^2).
 * https://oeis.org/A004295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 42*a(n-1) - a(n-2), a(0) = 1, a(1) = 44, a(2) = 1848
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 44;
    if (n == 2) return 1848;
    
    seq_t a_n_minus_2 = 44;    // a(1)
    seq_t a_n_minus_1 = 1848;  // a(2)
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t a_i = 42 * a_n_minus_1 - a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_i;
    }
    
    return a_n_minus_1;
}
