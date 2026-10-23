/*
 * A193663  Q-residue of A049310 (triangle of coefficients of Fibonacci
 *          polynomials), where Q is the triangle given by t(n,k)=k+1 for
 *          0<=k<=n. (See Comments.)
 * https://oeis.org/A193663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n)=2*a(n-1)+6*a(n-2)-7*a(n-3)-a(n-4) for n>=4 with initial
 *            values a(0)=0, a(1)=1, a(2)=1, a(3)=9
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 1;
    if (n == 3) return 9;

    seq_t a0 = 0;               /* a(n-4)   */
    seq_t a1 = 1;               /* a(n-3)   */
    seq_t a2 = 1;               /* a(n-2)   */
    seq_t a3 = 9;               /* a(n-1)   */
    seq_t curr;

    for (idx_t i = 4; i <= n; ++i) {
        /* recurrence: a(n) = 2*a(n-1) + 6*a(n-2) - 7*a(n-3) - a(n-4) */
        curr = a3 * (seq_t)2 + a2 * (seq_t)6;
        curr -= a1 * (seq_t)7;
        curr -= a0;

        /* shift window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = curr;
    }
    return a3;
}
