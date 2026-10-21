/*
 * A058645  a(n) = 2^(n-3)*n^2*(n+3).
 * https://oeis.org/A058645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) * n^2 * (n+3), with a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sn   = (seq_t)n;
    seq_t prod = sn * sn * (sn + (seq_t)3);

    if (n >= 3)
        return prod << (unsigned)(n - 3);   /* shift left by (n-3) */
    else
        return prod >> (unsigned)(3 - n);   /* shift right by (3-n) */
}
