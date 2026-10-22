/*
 * A002000  a(n+1) = a(n)*(a(n)^2 - 3) with a(0) = 7.
 * https://oeis.org/A002000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n)*(a(n)^2 - 3), a(0) = 7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)7;
    
    for (idx_t i = 0; i < n; i++) {
        seq_t sq = result * result;
        result = result * (sq - (seq_t)3);
    }
    
    return result;
}
