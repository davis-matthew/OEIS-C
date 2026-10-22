/*
 * A001793  a(n) = n*(n+3)*2^(n-3).
 * https://oeis.org/A001793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+3)*2^(n-3)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 5;
    
    seq_t n_seq = (seq_t)n;
    seq_t product = n_seq * (n_seq + 3);
    return product << (n - 3);
}
