/*
 * A003747  Number of perfect matchings (or domino tilings) in K_5 X P_2n.
 * https://oeis.org/A003747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 76*a(n-1) - 76*a(n-2) + a(n-3), a(1) = 56, a(2) = 4181,
 *            a(3) = 313501
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 56;
    if (n == 2) return 4181;
    if (n == 3) return 313501;
    
    seq_t a_n_minus_3 = 56;
    seq_t a_n_minus_2 = 4181;
    seq_t a_n_minus_1 = 313501;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t a_i = 76 * a_n_minus_1 - 76 * a_n_minus_2 + a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_i;
    }
    
    return a_n_minus_1;
}
