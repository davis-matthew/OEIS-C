/*
 * A002378  Oblong (or promic, pronic, or heteromecic) numbers: a(n) =
 *          n*(n+1).
 * https://oeis.org/A002378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n) = n*(n+1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n * ((seq_t)n + 1);
}
