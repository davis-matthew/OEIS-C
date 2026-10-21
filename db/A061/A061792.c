/*
 * A061792  a(n) = 49*(n*(n+1)/2) + 6.
 * https://oeis.org/A061792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 49 * (n*(n+1)/2) + 6
 * Range:     u8 n <= 2, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;   /* triangular number T(n) */
    return t * 49 + 6;
}
