/*
 * A057675  a(n) = 1 - (5/6)*n + (5/2)*n^2 + (10/3)*n^3 + n^4.
 * https://oeis.org/A057675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6 - 5*n + 15*n^2 + 20*n^3 + 6*n^4) / 6
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 172, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;           /* n converted to seq_t */
    seq_t num = 6;                  /* start with the constant term */

    /* subtract 5*n */
    num -= (seq_t)5 * x;

    /* compute n^2, n^3, n^4 progressively */
    seq_t x2 = x * x;               /* n^2 */
    num += (seq_t)15 * x2;          /* +15*n^2 */

    seq_t x3 = x2 * x;              /* n^3 */
    num += (seq_t)20 * x3;          /* +20*n^3 */

    seq_t x4 = x3 * x;              /* n^4 */
    num += (seq_t)6 * x4;           /* +6*n^4 */

    return num / 6;                 /* divide by 6, result is integer */
}
