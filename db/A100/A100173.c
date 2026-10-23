/*
 * A100173  Structured pentakis dodecahedral numbers (vertex structure 6).
 * https://oeis.org/A100173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/6)*(55*n^3 - 75*n^2 + 26*n)
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t val = 55 * n3 - 75 * n2 + 26 * N;
    return val / 6;
}
