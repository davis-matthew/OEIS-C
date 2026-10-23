/*
 * A154376  a(n) = 25*n^2 - 2*n.
 * https://oeis.org/A154376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 - 2*n
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)25 * nn * nn;
    val -= (seq_t)2 * nn;
    return val;
}
