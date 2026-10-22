/*
 * A007238  Length of longest chain of subgroups in S_n.
 * https://oeis.org/A007238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = ceiling(3*n/2) - A000120(n) - 1
 * Range:     u8 n <= 174, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    // Count number of 1's in binary representation of n
    seq_t temp = n_seq;
    seq_t bit_count = 0;
    while (temp > 0) {
        bit_count += temp & 1;
        temp >>= 1;
    }
    
    // a(n) = ceiling(3*n/2) - bit_count - 1
    return (3 * n_seq + 1) / 2 - bit_count - 1;
}
