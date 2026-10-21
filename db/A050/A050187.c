/*
 * A050187  a(n) = n * floor((n-1)/2).
 * https://oeis.org/A050187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * floor((n-1)/2)
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t t  = (seq_t)(n - 1) / 2;
    return nn * t;
}
