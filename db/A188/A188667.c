/*
 * A188667  Ordered (2,2)-selections from the multiset {1,1,2,2,3,3,...,n,n}.
 * https://oeis.org/A188667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+4)*(n-1)^2/4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 346, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)           /* a(0) = a(1) = 0 by definition */
        return 0;

    seq_t sn  = (seq_t)n;
    seq_t t1  = sn + (seq_t)4;           /* n + 4   */
    seq_t t2  = sn - (seq_t)1;           /* n - 1   */
    seq_t res = sn * t1;                 /* n*(n+4) */
    res *= t2;                           /* * (n-1) */
    res *= t2;                           /* * (n-1) again */
    res /= (seq_t)4;                     /* divide by 4 */
    return res;
}
