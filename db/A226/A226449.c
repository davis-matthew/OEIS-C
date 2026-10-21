/*
 * A226449  a(n) = n*(5*n^2-8*n+5)/2.
 * https://oeis.org/A226449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n^2 - 8*n + 5)/2
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 999, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t term = nn * ((seq_t)5 * nn * nn - (seq_t)8 * nn + (seq_t)5);
    return term / 2;
}
