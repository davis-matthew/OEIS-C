/*
 * A158776  a(n) = 80*n^2 + 1.
 * https://oeis.org/A158776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 80*n^2 + 1
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 7327, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;
    return nn * (seq_t)80 + (seq_t)1;
}
