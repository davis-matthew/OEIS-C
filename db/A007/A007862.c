/*
 * A007862  Number of triangular numbers that divide n.
 * https://oeis.org/A007862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|2n, d+1|2n} 1
 * Range:     u8 n <= 345, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t n_seq = (seq_t)n;
    
    for (idx_t k = 1; ; k++) {
        seq_t t_k = ((seq_t)k * ((seq_t)k + 1)) / 2;
        if (t_k > n_seq) break;
        if (n_seq % t_k == 0) {
            count++;
        }
    }
    
    return count;
}
