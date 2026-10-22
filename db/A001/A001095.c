/*
 * A001095  a(n) = n + n*(n-1)*(n-2)*(n-3)*(n-4).
 * https://oeis.org/A001095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + n*(n-1)*(n-2)*(n-3)*(n-4)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 86, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    if (n < 5) {
        return n_seq;
    }
    seq_t prod = n_seq * (n_seq - 1) * (n_seq - 2) * (n_seq - 3) * (n_seq - 4);
    return n_seq + prod;
}
