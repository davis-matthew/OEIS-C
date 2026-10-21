/*
 * A228307  The hyper-Wiener index of the Kneser graph K(n,2) (n>=5).
 * https://oeis.org/A228307
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*n*(n-1)*(n-2)*(n+9)
 * Range:     u8 none, u16 n <= 15, u32 n <= 261, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;                 /* behaviour below the offset is unspecified */
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn - 1);           /* n*(n-1)  */
    res *= (nn - 2);                     /* * (n-2)  */
    res *= (nn + 9);                     /* * (n+9)  */
    res /= 8;                            /* divide by 8 */
    return res;
}
