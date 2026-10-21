/*
 * A226470  a(n) = n^2 XOR triangular(n), where XOR is the bitwise logical
 *          exclusive-or operator.
 * https://oeis.org/A226470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n^2 XOR triangular(n) = n^2 XOR (n*(n+1)/2)
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    return sq ^ tri;
}
