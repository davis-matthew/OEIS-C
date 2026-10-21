/*
 * A081267  Diagonal of triangular spiral in A051682.
 * https://oeis.org/A081267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (9*n^2 + 7*n + 2)/2
 * Range:     u8 n <= 7, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t term = ((seq_t)9 * ns * ns + (seq_t)7 * ns + (seq_t)2) / (seq_t)2;
    return term;
}
