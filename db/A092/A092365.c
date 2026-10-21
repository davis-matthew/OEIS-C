/*
 * A092365  Coefficient of X^2 in expansion of (1 + n*X + n*X^2)^n.
 * https://oeis.org/A092365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(binomial(n, 2)+1) = n^2*(n*(n-1)/2 + 1)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 1, offset 1 */
    seq_t nn = (seq_t)n;            /* convert index to seq_t */
    seq_t t = nn * (nn - 1);        /* n*(n-1) in seq_t form */
    t /= 2;                         /* divide by 2 for binomial(n,2) */
    seq_t res = nn * nn;            /* n^2 */
    res *= (t + (seq_t)1);          /* multiply by (binomial(n,2)+1) */
    return res;
}
