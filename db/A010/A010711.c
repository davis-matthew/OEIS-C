/*
 * A010711  Period 2: repeat (4,6).
 * https://oeis.org/A010711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(-1)^n + 5
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return ((seq_t)4 + ((seq_t)2 * (n & 1)));
}
