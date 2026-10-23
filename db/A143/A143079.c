/*
 * A143079  a(n) = ((9+sqrt(9))^n + (9-sqrt(9))^n)/2.
 * https://oeis.org/A143079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 72*a(n-2), a(0)=1, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)9;   /* a(1) */
    seq_t a_cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        a_cur = a_prev1 * (seq_t)18 - a_prev2 * (seq_t)72;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_prev1;
}
