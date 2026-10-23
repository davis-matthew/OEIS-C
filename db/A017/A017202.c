/*
 * A017202  a(n) = (9*n + 3)^6.
 * https://oeis.org/A017202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 3)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 180, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (9*n + 3)^6 using only integer arithmetic */
    seq_t x  = (seq_t)n * 9 + 3;   /* x = 9*n + 3 */
    seq_t x2 = x * x;             /* x^2 */
    seq_t x4 = x2 * x2;           /* x^4 */
    return x4 * x2;               /* x^6 = x^4 * x^2 */
}
