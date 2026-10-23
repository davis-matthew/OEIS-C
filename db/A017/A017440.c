/*
 * A017440  a(n) = (11*n + 4)^4.
 * https://oeis.org/A017440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 4)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 22, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute x = 11*n + 4 */
    seq_t x = 11 * (seq_t)n + 4;

    /* Compute x^4 by repeated squaring */
    seq_t x2 = x * x;      /* x^2 */
    seq_t x4 = x2 * x2;    /* (x^2)^2 = x^4 */

    return x4;
}
