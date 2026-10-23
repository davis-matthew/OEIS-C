/*
 * A122876  a(0)=1, a(1)=1, a(2)=2, a(n) = a(n-1) - a(n-2) for n>2.
 * https://oeis.org/A122876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) - a(n-2); a(0) = 1, a(1) = 1, a(2) = 2
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
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
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;

    seq_t prevprev = (seq_t)1; /* a(1) */
    seq_t prev     = (seq_t)2; /* a(2) */
    seq_t curr;
    for (idx_t i = 3; i <= n; ++i)
    {
        curr = prev - prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}
