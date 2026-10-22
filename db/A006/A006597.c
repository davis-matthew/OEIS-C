/*
 * A006597  a(n) = n^2*(5*n-3)/2.
 * https://oeis.org/A006597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(5*n-3)/2
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 1197, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_squared = n_seq * n_seq;
    seq_t five_n_minus_3 = 5 * n_seq - 3;
    
    seq_t result;
    if (n_seq % 2 == 0) {
        result = (n_squared / 2) * five_n_minus_3;
    } else {
        result = n_squared * (five_n_minus_3 / 2);
    }
    
    return result;
}
