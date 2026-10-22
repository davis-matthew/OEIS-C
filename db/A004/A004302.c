/*
 * A004302  a(n) = n^2*(n+1)^2*(n+2)/12.
 * https://oeis.org/A004302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)^2*(n+2)/12
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_val = (seq_t)n;
    seq_t n_plus_1 = n_val + 1;
    seq_t n_plus_2 = n_val + 2;
    
    seq_t m = (n_val * n_plus_1) / 2;
    seq_t result = (m * m * n_plus_2) / 3;
    
    return result;
}
