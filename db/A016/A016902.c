/*
 * A016902  a(n) = (5*n + 4)^6.
 * https://oeis.org/A016902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 4)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 324, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index first as required, then compute the value. */
    seq_t x = 5 * (seq_t)n + 4;   /* base = 5*n + 4 */
    seq_t x2 = x * x;             /* (5*n + 4)^2 */
    seq_t x4 = x2 * x2;           /* (5*n + 4)^4 */
    return x4 * x2;               /* (5*n + 4)^6 = (5*n + 4)^4 * (5*n + 4)^2 */
}
