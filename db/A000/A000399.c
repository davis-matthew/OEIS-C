/*
 * A000399  Unsigned Stirling numbers of first kind s(n,3).
 * https://oeis.org/A000399
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-3)! + (2*n-3)*a(n-1) - (n-2)^2*a(n-2), n >= 5; a(3) =
 *            1, a(4) = 6
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 3) return 1;
    if (n == 4) return 6;
    
    seq_t a_n_minus_2 = 1;
    seq_t a_n_minus_1 = 6;
    seq_t fact = 2;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t i_m2 = (seq_t)(i - 2);
        seq_t a_n = fact + (seq_t)(2 * i - 3) * a_n_minus_1 - i_m2 * i_m2 * a_n_minus_2;
        
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
        fact = fact * i_m2;
    }
    
    return a_n_minus_1;
}
