/*
 * A172047  a(n) = n*(n+1)*(15*n^2-n-8)/12.
 * https://oeis.org/A172047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(15*n^2-n-8)/12
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn    = n;
    seq_t t     = sn * (sn + 1);            // n*(n+1)
    seq_t part  = 15 * sn * sn - sn - (seq_t)8; // 15*n^2 - n - 8
    return t * part / 12;
}
