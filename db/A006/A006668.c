/*
 * A006668  Exponential self-convolution of Pell numbers (divided by 2).
 * https://oeis.org/A006668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2) - 8*a(n-3), a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;
    
    seq_t a_n_minus_3 = 0;
    seq_t a_n_minus_2 = 0;
    seq_t a_n_minus_1 = 1;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t a_n = 6 * a_n_minus_1 - 4 * a_n_minus_2 - 8 * a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }
    
    return a_n_minus_1;
}
