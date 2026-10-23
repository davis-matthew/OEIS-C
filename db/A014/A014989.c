/*
 * A014989  a(n) = (1 - (-7)^n)/8.
 * https://oeis.org/A014989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1 - (-7)^n)/8
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 23, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=-6, a(n)= -6*a(n-1) + 7*a(n-2)   (n≥3) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset 1, but guard for safety */

    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)(-6);

    seq_t prev2 = (seq_t)1;          /* a(1) */
    seq_t prev1 = (seq_t)(-6);       /* a(2) */
    seq_t cur   = prev1;             /* placeholder */

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = - (seq_t)6 * prev1 + (seq_t)7 * prev2;
        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
