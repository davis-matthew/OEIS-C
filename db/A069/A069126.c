/*
 * A069126  Centered 13-gonal numbers.
 * https://oeis.org/A069126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (13*n*n - 13*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 99, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t val = 13 * nn * nn - 13 * nn + 2;
    return val / 2;
}
