/*
 * A019579  Place n distinguishable balls in n boxes (in n^n ways); let f(n,k)
 *          = number of ways that max in any box is k, for 1 <= k <= n;
 *          sequence gives f(n,n-2)/n.
 * https://oeis.org/A019579
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^3/2 for n >= 5; a(3)=2, a(4)=45
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)45;
    if (n < 3)  return (seq_t)0;        /* undefined below offset */

    seq_t nn = (seq_t)n;
    seq_t m  = (seq_t)(n - 1);
    seq_t m2 = m * m;                  /* (n-1)^2 */
    seq_t m3 = m2 * m;                 /* (n-1)^3 */
    seq_t res = nn * m3;               /* n*(n-1)^3 */
    return res / 2;                    /* divide by 2 */
}
