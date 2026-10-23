/*
 * A035328  a(n) = n*(2*n-1)*(2*n+1).
 * https://oeis.org/A035328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n-1)*(2*n+1)
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;   /* convert index to seq_t */
    seq_t sn2  = sn * (seq_t)2;   /* 2*n in seq_t */
    return sn * (sn2 - (seq_t)1) * (sn2 + (seq_t)1);
}
