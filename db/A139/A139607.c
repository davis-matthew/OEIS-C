/*
 * A139607  a(n) = 21*n + 7.
 * https://oeis.org/A139607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 21*n + 7
 * Range:     u8 n <= 11, u16 n <= 3120, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return ((seq_t)21 * n) + 7;
}
