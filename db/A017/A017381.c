/*
 * A017381  a(n) = (10*n + 9)^5.
 * https://oeis.org/A017381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 9)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 712, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n * 10 + 9;   /* 10*n + 9 */
    seq_t x2 = x * x;               /* (10*n + 9)^2 */
    seq_t x4 = x2 * x2;             /* (10*n + 9)^4 */
    return x4 * x;                  /* (10*n + 9)^5 */
}
