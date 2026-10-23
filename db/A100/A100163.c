/*
 * A100163  Structured disdyakis dodecahedral numbers (vertex structure 5).
 * https://oeis.org/A100163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (44*n^3 - 60*n^2 + 22*n)/6
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t term = 44 * n3 - 60 * n2 + 22 * nn;
    return term / 6;
}
