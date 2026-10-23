/*
 * A109116  a(n) = (n+1)^3*(n+2)^2*(n+5).
 * https://oeis.org/A109116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^3 * (n+2)^2 * (n+5)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 38, u64 n <= 1623,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n + 1;   /* n+1 */
    seq_t y = (seq_t)n + 2;   /* n+2 */
    seq_t z = (seq_t)n + 5;   /* n+5 */

    /* Compute x^3 = x*x*x */
    seq_t x2 = x * x;         /* x^2   */
    seq_t x3 = x2 * x;        /* x^3   */

    /* Compute y^2 */
    seq_t y2 = y * y;         /* y^2   */

    return x3 * y2 * z;
}
