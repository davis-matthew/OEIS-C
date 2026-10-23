/*
 * A026063  dot_product(n,n-1,...2,1)*(6,7,...,n,1,2,3,4,5).
 * https://oeis.org/A026063
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2+18*n-73)/6 (offset 6)
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* Compute n^2 + 18n - 73 first to keep intermediate sizes small */
    seq_t h = sn * sn + (seq_t)18 * sn - (seq_t)73;
    return sn * h / (seq_t)6;
}
