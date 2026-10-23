/*
 * A161734  a(n) = ((2+sqrt(2))*(5+sqrt(2))^n+(2-sqrt(2))*(5-sqrt(2))^n)/4.
 * https://oeis.org/A161734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 23*a(n-2) Initial values: a(0) = 1, a(1) = 6
 *            The sequence is defined for n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two base cases directly. */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    /* Use the linear recurrence to compute a(n) iteratively. */
    seq_t a_prevprev = (seq_t)1;  /* a(n-2) */
    seq_t a_prev = (seq_t)6;      /* a(n-1) */
    seq_t an = a_prev;            /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        an = a_prev * 10 - a_prevprev * 23;
        a_prevprev = a_prev;
        a_prev = an;
    }

    return an;
}
