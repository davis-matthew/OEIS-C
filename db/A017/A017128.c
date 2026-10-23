/*
 * A017128  a(n) = (8*n + 5)^4.
 * https://oeis.org/A017128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 5)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 31, u64 n <= 8191, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = x * (seq_t)8 + (seq_t)5;   /* Compute 8*n + 5 */
    seq_t y = x * x;              /* Square once */
    return y * y;                 /* Raise to the fourth power */
}
