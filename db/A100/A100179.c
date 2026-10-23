/*
 * A100179  Structured heptagonal diamond numbers (vertex structure 5).
 * https://oeis.org/A100179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/6)*(10*n^3 - 9*n^2 + 5*n)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 884, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;
    seq_t n3 = n2 * n0;
    seq_t term = 10 * n3 - 9 * n2 + 5 * n0;
    return term / 6;
}
