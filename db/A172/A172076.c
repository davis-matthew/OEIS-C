/*
 * A172076  a(n) = n*(n+1)*(14*n-11)/6.
 * https://oeis.org/A172076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(14*n-11)/6
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 877, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nS = (seq_t)n;          /* convert index to seq_t */
    seq_t n1 = nS + 1;
    seq_t t  = ((seq_t)14) * nS - 11;
    seq_t prod = nS * n1;          /* n*(n+1)   */
    prod *= t;                     /* * (14*n-11) */
    return prod / 6;               /* divide by 6 */
}
