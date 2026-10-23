/*
 * A158488  a(n) = 64*n^2 + 8.
 * https://oeis.org/A158488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64*n^2 + 8
 * Range:     u8 n <= 1, u16 n <= 31, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * (seq_t)n;   /* n^2 */
    t = t * 64 + 8;                  /* 64*n^2 + 8 */
    return t;
}
