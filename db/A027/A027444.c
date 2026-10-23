/*
 * A027444  a(n) = n^3 + n^2 + n.
 * https://oeis.org/A027444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + n^2 + n
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t sq = ns * ns;          /* n^2 */
    seq_t csq = ns * sq;         /* n^3 */
    return csq + sq + ns;
}
