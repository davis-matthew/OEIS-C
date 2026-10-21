/*
 * A239325  a(n) = 6*n^2 + 8*n + 1.
 * https://oeis.org/A239325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 + 8*n + 1
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    seq_t u = t * 6 + 8;   /* 6*n + 8 */
    return t * u + 1;      /* n*(6*n + 8) + 1 = 6*n^2 + 8*n + 1 */
}
