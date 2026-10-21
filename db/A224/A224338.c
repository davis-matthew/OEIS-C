/*
 * A224338  Number of idempotent 7 X 7 0..n matrices of rank 6.
 * https://oeis.org/A224338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n^6 + 84*n^5 + 210*n^4 + 280*n^3 + 210*n^2 + 84*n + 7
 *            (offset 1)
 * Range:     u8 none, u16 n <= 3, u32 n <= 24, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used – offset is 1 */

    seq_t x  = (seq_t)n;             /* n */
    seq_t x2 = x * x;                /* n^2 */
    seq_t x3 = x2 * x;               /* n^3 */
    seq_t x4 = x3 * x;               /* n^4 */
    seq_t x5 = x4 * x;               /* n^5 */
    seq_t x6 = x5 * x;               /* n^6 */

    return (seq_t)14 * x6
         + (seq_t)84 * x5
         + (seq_t)210 * x4
         + (seq_t)280 * x3
         + (seq_t)210 * x2
         + (seq_t)84  * x
         + (seq_t)7;
}
