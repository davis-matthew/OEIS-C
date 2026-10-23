/*
 * A100705  a(n) = n^3 + (n+1)^2.
 * https://oeis.org/A100705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + (n+1)^2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;
    seq_t square = (nn + 1) * (nn + 1);
    return cube + square;
}
