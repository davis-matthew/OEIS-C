/*
 * A000567  Octagonal numbers: n*(3*n-2). Also called star numbers.
 * https://oeis.org/A000567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(3*n-2)
 * Range:     u8 n <= 9, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return n_seq * (3 * n_seq - 2);
}
