/*
 * A185096  Let T(n) = n(n+1)/2 be the n-th triangular number (A000217); a(n)
 *          = T(8T(n)).
 * https://oeis.org/A185096
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n + 1)*(2*n + 1)^2
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 151, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t ans = sn * (sn + 1);          /* n*(n+1)          */
    seq_t two_n_plus1 = 2 * sn + 1;     /* 2*n + 1          */
    ans = ans * two_n_plus1;           /* n(n+1)(2n+1)     */
    ans = ans * two_n_plus1;           /* n(n+1)(2n+1)^2   */
    ans = ans * 2;                     /* 2*n(n+1)(2n+1)^2 */
    return ans;
}
