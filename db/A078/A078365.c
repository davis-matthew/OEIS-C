/*
 * A078365  A Chebyshev T-sequence with Diophantine property.
 * https://oeis.org/A078365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=15, a(n)=15*a(n-1)-a(n-2) for n>=2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)15;

    /* iterate the recurrence up to n */
    seq_t a_prev2 = (seq_t)2;   /* a(0) */
    seq_t a_prev1 = (seq_t)15;  /* a(1) */
    seq_t a_cur   = a_prev1;    /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        a_cur = (seq_t)15 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_cur;
}
