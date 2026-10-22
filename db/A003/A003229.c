/*
 * A003229  a(n) = a(n-1) + 2*a(n-3) with a(0)=a(1)=1, a(2)=3.
 * https://oeis.org/A003229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3), a(0) = 1, a(1) = 1, a(2) = 3
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 42, u64 n <= 84, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    
    seq_t a_i_minus_3 = (seq_t)1;
    seq_t a_i_minus_2 = (seq_t)1;
    seq_t a_i_minus_1 = (seq_t)3;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t a_i = a_i_minus_1 + 2 * a_i_minus_3;
        a_i_minus_3 = a_i_minus_2;
        a_i_minus_2 = a_i_minus_1;
        a_i_minus_1 = a_i;
    }
    
    return a_i_minus_1;
}
