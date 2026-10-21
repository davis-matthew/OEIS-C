/*
 * A053730  a(n) = 2^(n-2)*(n^2 - n + 4).
 * https://oeis.org/A053730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) * (n^2 - n + 4) (offset 0)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    seq_t coeff = (seq_t)n * (seq_t)n - (seq_t)n + (seq_t)4;
    return ((seq_t)1 << (n - 2)) * coeff;
}
