/*
 * A002866  a(0) = 1; for n > 0, a(n) = 2^(n-1)*n!.
 * https://oeis.org/A002866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; for n > 0, a(n) = 2^(n-1) * n!
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    seq_t result = 1;
    
    for (idx_t i = 1; i < n; i++) {
        seq_t i_seq = (seq_t)i;
        result *= 2 * i_seq;
    }
    result *= (seq_t)n;
    
    return result;
}
