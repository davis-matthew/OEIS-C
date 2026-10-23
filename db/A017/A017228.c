/*
 * A017228  a(n) = (9*n + 5)^8.
 * https://oeis.org/A017228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 5)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 27, u128 n <= 7281,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base: x = 9*n + 5 */
    seq_t x = (seq_t)n * 9 + 5;

    /* Compute x^8 by repeated squaring */
    seq_t x2 = x * x;      /* x^2   */
    seq_t x4 = x2 * x2;    /* x^4   */
    seq_t x8 = x4 * x4;    /* x^8   */

    return x8;
}
