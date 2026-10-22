/*
 * A006527  a(n) = (n^3 + 2*n)/3.
 * https://oeis.org/A006527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n^3 + 2*n)/3
 * Range:     u8 n <= 9, u16 n <= 58, u32 n <= 2344, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;
    
    if (n % 3 == 0) {
        return (n_seq / 3) * (n_sq + 2);
    } else {
        return n_seq * ((n_sq + 2) / 3);
    }
}
