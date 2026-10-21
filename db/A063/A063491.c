/*
 * A063491  a(n) = (2*n - 1)*(3*n^2 - 3*n + 2)/2.
 * https://oeis.org/A063491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 1)*(3*n^2 - 3*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 910, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term = ((seq_t)2 * sn - 1) *
                 ((seq_t)3 * sn * sn - (seq_t)3 * sn + (seq_t)2) /
                 (seq_t)2;
    return term;
}
