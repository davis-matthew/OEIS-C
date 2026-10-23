/*
 * A117625  Maximum number of regions defined by n zigzag-lines in the plane
 *          when a zigzag-line is defined as consisting of two parallel
 *          infinite half-lines joined by a straight line segment.
 * https://oeis.org/A117625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*n^2/2 - 7*n/2 + 1
 * Range:     u8 n <= 5, u16 n <= 121, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t nn = sn * sn;
    seq_t term1 = (seq_t)9 * nn;
    seq_t term2 = (seq_t)7 * sn;
    seq_t val = (term1 - term2 + (seq_t)2) / (seq_t)2;
    return val;
}
