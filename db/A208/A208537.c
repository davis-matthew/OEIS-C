/*
 * A208537  Number of 7-bead necklaces of n colors not allowing reversal, with
 *          no adjacent beads having the same color.
 * https://oeis.org/A208537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)^7 - (n-1))/7
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 24, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)(n - 1);
    seq_t pow = m;
    for (idx_t i = 1; i < 7; ++i) {
        pow *= m;
    }
    return (pow - m) / 7;
}
