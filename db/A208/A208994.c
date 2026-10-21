/*
 * A208994  Number of 3-bead necklaces labeled with numbers -n..n not allowing
 *          reversal, with sum zero and first differences in -n..n.
 * https://oeis.org/A208994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + floor(n*(n+1)/3)
 * Range:     u8 n <= 15, u16 n <= 441, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);
    seq_t div = prod / 3;
    return div + 1;
}
