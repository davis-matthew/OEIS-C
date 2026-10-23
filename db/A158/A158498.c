/*
 * A158498  a(n) = (1/2)*(n^3 - 6*n^2 + 13*n - 6).
 * https://oeis.org/A158498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)*(n^3 - 6*n^2 + 13*n - 6)
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;
    seq_t term = ((sn - (seq_t)1) * (sn - (seq_t)2) * (sn - (seq_t)3))
                 + ((seq_t)2 * sn);
    return term / (seq_t)2;
}
