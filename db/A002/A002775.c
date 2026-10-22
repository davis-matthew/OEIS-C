/*
 * A002775  a(n) = n^2 * n!.
 * https://oeis.org/A002775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * n!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    seq_t n_seq = (seq_t)n;
    
    for (seq_t i = 2; i <= n_seq; i++) {
        result *= i;
    }
    
    result *= n_seq * n_seq;
    
    return result;
}
