/*
 * A160174  a(n) = (2*n - 1)*(24*n^2 - 42*n + 19).
 * https://oeis.org/A160174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*n^3 - 108*n^2 + 80*n - 19
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 448, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is 1-indexed; the formula is valid for all n ≥ 1 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    return (48 * n3) - (108 * n2) + (80 * nn) - (19);
}
