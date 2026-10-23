/*
 * A158574  a(n) = 256*n^2 + 16.
 * https://oeis.org/A158574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 256*n^2 + 16
 * Range:     u8 n <= 0, u16 n <= 15, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;            /* convert index to seq_t */
    return (seq_t)256 * x * x + (seq_t)16;
}
