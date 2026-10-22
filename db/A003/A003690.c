/*
 * A003690  Number of spanning trees in K_3 X P_n.
 * https://oeis.org/A003690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - 24*a(n-2) + a(n-3), n > 3; a(1) = 3, a(2) =
 *            75, a(3) = 1728
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)75;
    if (n == 3) return (seq_t)1728;
    
    seq_t a_n_minus_3 = (seq_t)3;
    seq_t a_n_minus_2 = (seq_t)75;
    seq_t a_n_minus_1 = (seq_t)1728;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t a_n = (seq_t)24 * a_n_minus_1 - (seq_t)24 * a_n_minus_2 + a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }
    
    return a_n_minus_1;
}
