/*
 * A004278  1, 3 and the positive even numbers.
 * https://oeis.org/A004278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (abs(4 - n) + 3*n - 4)/2
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t diff = (n_seq >= 4) ? (n_seq - 4) : (4 - n_seq);
    return (diff + 3 * n_seq - 4) / 2;
}
