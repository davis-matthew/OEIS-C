/*
 * A190969  a(n) = 5*a(n-1) - 8*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) with a(0)=0, a(1)=1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 21, i64 n <= 42, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values */
    if (n == 0)   return (seq_t)0;
    if (n == 1)   return (seq_t)1;

    /* Iterative computation of the recurrence */
    seq_t prev = (seq_t)0;   /* a(0) */
    seq_t cur  = (seq_t)1;   /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* next = 5*cur - 8*prev  */
        next = (seq_t)5 * cur - (seq_t)8 * prev;
        prev = cur;
        cur  = next;
    }
    return cur;
}
