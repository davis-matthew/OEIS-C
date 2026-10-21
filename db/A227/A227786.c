/*
 * A227786  Take squares larger than 1, subtract 3 from even squares and 2
 *          from odd squares.
 * https://oeis.org/A227786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000290(n+1) - 2 - (n mod 2)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Perform the computation in seq_t to avoid overflow on small types. */
    seq_t m = (seq_t)n + 1;
    return m * m - 2 - ((seq_t)n & 1);
}
