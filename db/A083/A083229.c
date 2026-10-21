/*
 * A083229  a(n) = (3*3^n + (-5)^n)/4.
 * https://oeis.org/A083229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (3*3^n + (-5)^n)/4
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 27, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1, a(n) = -2*a(n-1) + 15*a(n-2) */
seq_t a(idx_t n)
{
    /* Handle the offseted first two terms */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    /* a(0) and a(1) */
    seq_t a_prev2 = (seq_t)1;   /* a(n-2) */
    seq_t a_prev1 = (seq_t)1;   /* a(n-1) */
    seq_t a_cur   = 0;          /* placeholder for current term */

    /* Iterate from i=2 up to n using the linear recurrence */
    for (idx_t i = 2; i <= n; ++i)
    {
        a_cur = a_prev1 * (seq_t)-2 + a_prev2 * (seq_t)15;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}
