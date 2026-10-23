/*
 * A008574  a(0) = 1, thereafter a(n) = 4n.
 * https://oeis.org/A008574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1, a(n) = 4*n for n > 0
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return (seq_t)4 * (seq_t)n;
}
