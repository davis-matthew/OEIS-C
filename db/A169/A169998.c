/*
 * A169998  a(0)=1, a(1)=1; thereafter a(n) = -a(n-1) - 2*a(n-2).
 * https://oeis.org/A169998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 29, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1; a(n) = -a(n-1) - 2*a(n-2) for n>=2 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)1;

    seq_t a_prev2 = (seq_t)1; /* a(0) */
    seq_t a_prev1 = (seq_t)1; /* a(1) */
    seq_t an = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i)
    {
        an = -a_prev1 - (a_prev2 * 2);
        a_prev2 = a_prev1;
        a_prev1 = an;
    }

    return an;
}
