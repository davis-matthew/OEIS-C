/*
 * A000969  Expansion of g.f. (1 + x + 2*x^2)/((1 - x)^2*(1 - x^3)).
 * https://oeis.org/A000969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2*n+3)*(n+1)/3)
 * Range:     u8 n <= 10, u16 n <= 311, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t numerator = (2 * n_seq + 3) * (n_seq + 1);
    return numerator / 3;
}
