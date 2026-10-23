/*
 * A152619  a(n) = n*(n+2)^2.
 * https://oeis.org/A152619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)^2
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;            /* convert index to seq_t */
    seq_t y = x + (seq_t)2;        /* n + 2 */
    return x * y * y;              /* n * (n+2)^2 */
}
