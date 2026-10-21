/*
 * A071239  a(n) = n*(n+1)*(n^2 + 2)/6.
 * https://oeis.org/A071239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2 + 2)/6
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 393, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t  = nn * (nn + 1);   /* n*(n+1)  */
    t /= 2;                     /* divide by 2   */
    seq_t n2 = nn * nn + 2;     /* n^2 + 2      */
    t *= n2;                    /* multiply by (n^2+2) */
    t /= 3;                     /* divide by 3   */
    return t;
}
