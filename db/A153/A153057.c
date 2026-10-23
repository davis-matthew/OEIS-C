/*
 * A153057  a(0)=3; a(n) = n^2 + a(n-1) for n>0.
 * https://oeis.org/A153057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + n*(n+1)*(2*n+1)/6
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t before multiplication */
    seq_t term = nn * (nn + 1) * (2 * nn + 1); /* n*(n+1)*(2n+1) */
    term /= 6;                   /* divide by 6 – the product is always divisible by 6 */
    term += 3;                   /* add the initial offset 3 */
    return term;
}
