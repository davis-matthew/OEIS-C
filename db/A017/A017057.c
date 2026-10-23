/*
 * A017057  a(n) = (7*n + 6)^5.
 * https://oeis.org/A017057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 6)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 11, u64 n <= 1017, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n * 7 + 6;   /* compute 7*n + 6 */
    seq_t x2 = x * x;             /* (7*n+6)^2 */
    seq_t x4 = x2 * x2;           /* (7*n+6)^4 */
    return x4 * x;                /* (7*n+6)^5 */
}
