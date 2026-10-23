/*
 * A167149  10000-gonal numbers: a(n) = n + 4999 * n * (n-1).
 * https://oeis.org/A167149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 4999*n*(n-1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 927, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t tmp = sn * (sn - (seq_t)1);   /*  n*(n-1)  */
    tmp *= (seq_t)4999;                /*  4999*n*(n-1)  */
    return sn + tmp;                   /*  final value  */
}
