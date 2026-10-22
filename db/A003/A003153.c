/*
 * A003153  a(n) = integer nearest n*(1+sqrt(2)).
 * https://oeis.org/A003153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = integer nearest n*(1+sqrt(2))
 * Range:     u8 n <= 11, u16 n <= 1746, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;
    seq_t two_n_sq = n_sq + n_sq;
    
    // Binary search for floor(sqrt(2*n^2))
    seq_t low = 0, high = n_seq << 1;
    while (low < high) {
        seq_t mid = (low + high + 1) / 2;
        if (mid * mid <= two_n_sq) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    
    seq_t s = low;
    
    // Round n*sqrt(2): if 2*n^2 > s^2 + s, increment s
    if (two_n_sq > s * s + s) {
        s = s + 1;
    }
    
    // a(n) = n + round(n*sqrt(2))
    return n_seq + s;
}
