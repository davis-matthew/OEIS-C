/*
 * A022271  a(n) = n*(13*n + 1)/2.
 * https://oeis.org/A022271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n + 1)/2
 * Range:     u8 n <= 6, u16 n <= 100, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t temp = (seq_t)13 * nn + (seq_t)1;
    return nn * temp / (seq_t)2;
}
