/*
 * A006325  4-dimensional analog of centered polygonal numbers: a(n) =
 *          n*(n-1)*(n^2-n+1)/6.
 * https://oeis.org/A006325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^2-n+1)/6
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 401, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_minus_1 = n_seq - 1;
    seq_t n_squared = n_seq * n_seq;
    seq_t n2_minus_n_plus_1 = n_squared - n_seq + 1;
    
    seq_t half = (n_seq * n_minus_1) / 2;
    
    if (half % 3 == 0) {
        return (half / 3) * n2_minus_n_plus_1;
    } else {
        return half * (n2_minus_n_plus_1 / 3);
    }
}
