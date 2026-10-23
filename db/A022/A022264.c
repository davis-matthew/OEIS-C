/*
 * A022264  a(n) = n*(7*n - 1)/2.
 * https://oeis.org/A022264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n - 1)/2
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)7 * nn - (seq_t)1) / (seq_t)2;
}
