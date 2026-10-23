/*
 * A158492  a(n) = 100*n^2 + 10.
 * https://oeis.org/A158492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n^2 + 10
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 6553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = x * x;        /* n^2 */
    x = x * 100;      /* 100*n^2 */
    x = x + 10;       /* +10 */
    return x;
}
