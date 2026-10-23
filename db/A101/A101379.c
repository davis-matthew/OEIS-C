/*
 * A101379  a(n) = n*(n+1)^2*(6*n^3-5*n^2+3*n+2)/24.
 * https://oeis.org/A101379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)^2*(6*n^3-5*n^2+3*n+2)/24
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t sn1  = sn + ((seq_t)1);
    seq_t sn1sq= sn1 * sn1;
    seq_t sn2  = sn * sn;
    seq_t sn3  = sn2 * sn;
    seq_t poly = (seq_t)6 * sn3 - (seq_t)5 * sn2 + (seq_t)3 * sn + ((seq_t)2);
    seq_t prod = sn * sn1sq * poly;
    seq_t result = prod / ((seq_t)24);
    return result;
}
