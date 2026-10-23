/*
 * A017032  a(n) = (7*n + 4)^4.
 * https://oeis.org/A017032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 35, u64 n <= 9361, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;     /* convert index to seq_t */
    x = x * 7 + 4;          /* x = 7*n + 4 */
    seq_t y = x * x;        /* y = (7*n + 4)^2 */
    y = y * y;              /* y = (7*n + 4)^4 */
    return y;
}
