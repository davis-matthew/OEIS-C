/*
 * A016191  Expansion of g.f. 1/((1-9*x)*(1-12*x)).
 * https://oeis.org/A016191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) + 9^n ; a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)21;
    seq_t a_prev2 = (seq_t)1;    /* a(0)  */
    seq_t a_prev1 = (seq_t)21;   /* a(1)  */
    seq_t pow9    = (seq_t)9;    /* 9^1   */
    for (idx_t i = 2; i <= n; ++i) {
        pow9   *= (seq_t)9;                         /* 9^i   */
        seq_t cur = (seq_t)12 * a_prev1 + pow9;    /* a(i)  */
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
