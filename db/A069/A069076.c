/*
 * A069076  a(n) = (4*n^2 - 1)^3.
 * https://oeis.org/A069076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (4*n^2 - 1)^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 20, u64 n <= 812, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;          /* Convert index to seq_t */
    t = 4 * t * t - 1;           /* Compute 4*n^2 - 1 */
    return t * t * t;            /* Return its cube */
}
