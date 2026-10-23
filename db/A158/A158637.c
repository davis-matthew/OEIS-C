/*
 * A158637  a(n) = 576*n^2 + 24.
 * https://oeis.org/A158637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 576*n^2 + 24, offset 0
 * Range:     u8 n <= 0, u16 n <= 10, u32 n <= 2730, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * (seq_t)n;   /* n^2 */
    val *= (seq_t)576;                 /* 576 * n^2 */
    val += (seq_t)24;                  /* + 24 */
    return val;
}
