/*
 * A007202  Crystal ball sequence for hexagonal close-packing.
 * https://oeis.org/A007202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 7*(2*n+1)*(2*n^2+2*n+1)/8 + (-1)^n/8
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 573, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    
    seq_t numerator = 28 * n3 + 42 * n2 + 28 * n_seq + 7;
    
    if (n & 1) {
        numerator -= 1;
    } else {
        numerator += 1;
    }
    
    return numerator / 8;
}
