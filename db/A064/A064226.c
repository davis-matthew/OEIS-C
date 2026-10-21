/*
 * A064226  a(n) = (9*n^2 + 13*n + 6)/2.
 * https://oeis.org/A064226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2 + 13*n + 6)/2
 * Range:     u8 n <= 6, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t term = ((seq_t)9 * ns * ns + (seq_t)13 * ns + 6) / 2;
    return term;
}
