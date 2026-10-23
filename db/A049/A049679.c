/*
 * A049679  a(n) = (F(8*n+7)+F(8*n+5))/3, where F=A000045 (the Fibonacci
 *          sequence).
 * https://oeis.org/A049679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) - a(n-2), a(0) = 6, a(1) = 281
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases */
    if (n == 0)
        return (seq_t)6;
    if (n == 1)
        return (seq_t)281;

    seq_t a_prev2 = (seq_t)6;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)281; /* a(n-1) */
    seq_t an = a_prev1;         /* placeholder for a(n) */

    /* Iterate from 2 to n, using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)47 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = an;
    }
    return an;
}
