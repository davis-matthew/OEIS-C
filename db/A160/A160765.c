/*
 * A160765  Expansion of (1+13*x+32*x^2+13*x^3+x^4)/(1-x)^5.
 * https://oeis.org/A160765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+n+1)*(5*n^2+5*n+2)/2
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 202, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t term1 = sn * sn + sn + 1;
    seq_t term2 = 5 * sn * sn + 5 * sn + 2;
    return term1 * term2 / 2;
}
