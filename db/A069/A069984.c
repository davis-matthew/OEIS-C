/*
 * A069984  a(n) = 1123 + 21460*n.
 * https://oeis.org/A069984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1123 + 21460*n
 * Range:     u8 none, u16 n <= 3, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)1123 + ((seq_t)n * 21460);
}
