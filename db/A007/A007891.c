/*
 * A007891  A Kutz sequence.
 * https://oeis.org/A007891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 2*floor(n/3))^2
 * Range:     u8 n <= 43, u16 n <= 763, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t temp = n_seq - 2 * (n_seq / 3);
    return temp * temp;
}
