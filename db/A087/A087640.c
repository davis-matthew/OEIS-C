/*
 * A087640  To obtain a(n+1), take the square of the n-th partial sum, minus
 *          the sum of the first n squared terms, then divide this difference
 *          by a(n); for all n>1, starting with a(0)=1, a(1)=1.
 * https://oeis.org/A087640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=5, a(n)=a(n-1)+3*a(n-2)-a(n-3) for
 *            n>3
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the four seed values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* For n >= 4, iterate the recurrence using only three
       consecutive terms. */
    seq_t a_prev3 = (seq_t)1;  /* a(n-3)  */
    seq_t a_prev2 = (seq_t)2;  /* a(n-2)  */
    seq_t a_prev1 = (seq_t)5;  /* a(n-1)  */
    seq_t a_curr  = (seq_t)0;  /* a(n)     */

    for (idx_t i = 4; i <= n; ++i)
    {
        a_curr  = a_prev1 + (seq_t)3 * a_prev2 - a_prev3;
        a_prev3 = a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
