/*
 * A132762  a(n) = n*(n + 19).
 * https://oeis.org/A132762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+19)
 * Range:     u8 n <= 9, u16 n <= 246, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n * ((seq_t)n + (seq_t)19);
}
