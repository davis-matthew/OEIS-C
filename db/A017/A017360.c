/*
 * A017360  a(n) = (10*n + 7)^8.
 * https://oeis.org/A017360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 7)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 24, u128 n <= 6552,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute x = 10*n + 7 in seq_t */
    seq_t x = (seq_t)n * 10 + 7;

    /* Square twice to obtain the 8th power */
    seq_t x2 = x * x;      /* x^2 */
    seq_t x4 = x2 * x2;    /* x^4 */
    seq_t x8 = x4 * x4;    /* x^8 */

    return x8;
}
