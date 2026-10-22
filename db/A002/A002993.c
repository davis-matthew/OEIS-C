/*
 * A002993  Initial digits of squares.
 * https://oeis.org/A002993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000030(A000290(n)) = first digit of n^2
 * Range:     u8 n <= 15, u16 n <= 351, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t square = (seq_t)n * ((seq_t)n);
    while (square >= 10) {
        square /= 10;
    }
    return square;
}
