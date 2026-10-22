/*
 * A007481  Number of subsequences of [ 1,...,n ] in which each even number
 *          has an odd neighbor.
 * https://oeis.org/A007481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-4), a(0) = 1, a(1) = 2, a(2) = 3, a(3) =
 *            7
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 70, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 3;
    if (n == 3) return 7;
    
    seq_t a_n_minus_4 = 1;
    seq_t a_n_minus_3 = 2;
    seq_t a_n_minus_2 = 3;
    seq_t a_n_minus_1 = 7;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t a_n = 3 * a_n_minus_2 + 2 * a_n_minus_4;
        a_n_minus_4 = a_n_minus_3;
        a_n_minus_3 = a_n_minus_2;
        a_n_minus_2 = a_n_minus_1;
        a_n_minus_1 = a_n;
    }
    
    return a_n_minus_1;
}
