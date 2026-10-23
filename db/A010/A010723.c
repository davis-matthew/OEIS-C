/*
 * A010723  Period 2: repeat (6,7).
 * https://oeis.org/A010723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(1/2)*(-1)^n + 13/2 => 6 if n even, 7 if n odd
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)(6 + (n & 1));
}
