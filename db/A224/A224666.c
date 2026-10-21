/*
 * A224666  Number of 4 X 4 0..n matrices with each 2 X 2 subblock idempotent.
 * https://oeis.org/A224666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 + 24*n + 52
 * Range:     u8 n <= 5, u16 n <= 175, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * nn) * 2 + nn * 24 + 52;
}
