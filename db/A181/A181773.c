/*
 * A181773  Molecular topological indices of the cocktail party graphs.
 * https://oeis.org/A181773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(n-1)*n*(2n-1)
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return (seq_t)8 * (nn - 1) * nn * (2 * nn - 1);
}
