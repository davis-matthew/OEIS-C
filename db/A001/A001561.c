/*
 * A001561  a(n) = (7*n+3)*(7*n+5)*(7*n+6).
 * https://oeis.org/A001561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n+3)*(7*n+5)*(7*n+6)
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 231, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t seven_n = 7 * n_seq;
    return (seven_n + 3) * (seven_n + 5) * (seven_n + 6);
}
