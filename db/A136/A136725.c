/*
 * A136725  Primitive dimensions of Hadamard matrices.
 * https://oeis.org/A136725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 for n = 1, 2 for n = 2, 8*n – 12 for n ≥ 3
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    return (seq_t)8 * (seq_t)n - (seq_t)12;
}
