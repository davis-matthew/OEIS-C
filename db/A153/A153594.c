/*
 * A153594  a(n) = ((4 + sqrt(3))^n - (4 - sqrt(3))^n)/(2*sqrt(3)).
 * https://oeis.org/A153594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((4 + sqrt(3))^n - (4 - sqrt(3))^n)/(2*sqrt(3)).
 *            Recurrence: a(0) = 0, a(1) = 1, a(n) = 8*a(n-1) - 13*a(n-2) for
 *            n > 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1.  The sequence is defined for n >= 1. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Base values for the recurrence.  a(2) = 8. */
    seq_t a_prevprev = (seq_t)1;   /* a(1) */
    seq_t a_prev     = (seq_t)8;   /* a(2) */
    if (n == (idx_t)2)
        return a_prev;

    /* Iterate the recurrence until we reach the desired index. */
    for (idx_t i = (idx_t)3; i <= n; ++i)
    {
        seq_t a_next = (seq_t)8 * a_prev - (seq_t)13 * a_prevprev;
        a_prevprev = a_prev;
        a_prev     = a_next;
    }
    return a_prev;
}
