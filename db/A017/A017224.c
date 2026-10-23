/*
 * A017224  a(n) = (9*n + 5)^4.
 * https://oeis.org/A017224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 5)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 27, u64 n <= 7281, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = n;       /* convert index to seq_t */
    x *= 9;            /* 9 * n   */
    x += 5;            /* 9*n + 5 */
    seq_t y = x * x;   /* (9*n+5)^2 */
    return y * y;      /* (9*n+5)^4 */
}
