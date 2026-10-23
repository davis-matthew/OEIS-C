/*
 * A134481  Row sums of triangle A134480.
 * https://oeis.org/A134481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 * binomial(n+2,3)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n8 = (seq_t)n;
    seq_t t = n8 * (n8 + 1);    /* n*(n+1) */
    t *= (n8 + 2);             /* * (n+2) */
    t /= (seq_t)6;             /* divide by 6 */
    t *= (seq_t)5;             /* multiply by 5 */
    return t;
}
