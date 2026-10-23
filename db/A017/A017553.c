/*
 * A017553  a(n) = (12*n + 2)^9.
 * https://oeis.org/A017553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 2)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 11, u128 n <= 1592,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base: 12*n + 2 */
    seq_t x = (seq_t)n * 12 + 2;

    /* compute x^3 */
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;

    /* compute x^6 = (x^3)^2 */
    seq_t x6 = x3 * x3;

    /* compute x^9 = x^6 * x^3 */
    seq_t x9 = x6 * x3;

    return x9;
}
