/*
 * A002801  a(n) = (2*n-1)*a(n-1) - (n-1)*a(n-2) with a(0) = a(1) = 1.
 * https://oeis.org/A002801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) - (n-1)*a(n-2), a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    
    seq_t prev_prev = 1;
    seq_t prev = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t i_seq = (seq_t)i;
        seq_t current = (2 * i_seq - 1) * prev - (i_seq - 1) * prev_prev;
        prev_prev = prev;
        prev = current;
    }
    
    return prev;
}
