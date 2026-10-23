/*
 * A155212  a(n) = (n^2 + 9*n + 4)/2.
 * https://oeis.org/A155212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 9*n + 4)/2
 * Range:     u8 n <= 18, u16 n <= 357, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * (nn + (seq_t)9) + (seq_t)4) / (seq_t)2;
}
