/*
 * A006566  Dodecahedral numbers: a(n) = n*(3*n - 1)*(3*n - 2)/2.
 * https://oeis.org/A006566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(3*n - 1)*(3*n - 2)/2
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 984, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t term1 = n_seq;
    seq_t term2 = 3 * n_seq - 1;
    seq_t term3 = 3 * n_seq - 2;
    
    if (n_seq % 2 == 0) {
        term3 /= 2;
    } else {
        term2 /= 2;
    }
    
    return term1 * term2 * term3;
}
