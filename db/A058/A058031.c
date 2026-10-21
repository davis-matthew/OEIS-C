/*
 * A058031  a(n) = n^4 - 2*n^3 + 3*n^2 - 2*n + 1, the Alexander polynomial for
 *          reef and granny knots.
 * https://oeis.org/A058031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - n + 1)^2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn * nn - nn + 1;  /* intermediate (n^2 - n + 1) */
    return t * t;                  /* final square */
}
