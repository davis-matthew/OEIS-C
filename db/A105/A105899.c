/*
 * A105899  Period 6: repeat [1, 1, 2, 2, 3, 3].
 * https://oeis.org/A105899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less, changed
 * Formula:   a(n) = floor(n/2) - 3*floor(n/6) + 1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t half  = n / 2;
    idx_t sixth = n / 6;
    return (seq_t)half - (seq_t)(3 * sixth) + 1;
}
