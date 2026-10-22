/*
 * A001470  Number of degree-n permutations of order dividing 3.
 * https://oeis.org/A001470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, changed
 * Formula:   a(n) = a(n-1) + (n-1)*(n-2)*a(n-3), a(0) = 1, a(1) = 1, a(2) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) {
        return 1;
    }
    
    seq_t a0 = 1, a1 = 1, a2 = 1;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t i_minus_1 = (seq_t)(i - 1);
        seq_t i_minus_2 = (seq_t)(i - 2);
        seq_t a_i = a2 + i_minus_1 * i_minus_2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_i;
    }
    
    return a2;
}
