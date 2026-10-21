/*
 * A053987  Numerators of successive convergents to tan(1/2) using continued
 *          fraction 1/(2-1/(6-1/(10-1/(14-1/(18-1/(22-1/(26-1/30-...))))))).
 * https://oeis.org/A053987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n+1) = (4*n+2)*a(n) - a(n-1), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* not part of the sequence, but harmless */
    if (n == 1) return (seq_t)1;            /* a(1) = 1 */

    seq_t a_prev = (seq_t)0;    /* a(0) */
    seq_t a_curr = (seq_t)1;    /* a(1) */

    for (idx_t i = 1; i < n; ++i)          /* compute up to a(n) */
    {
        seq_t coef   = (seq_t)4 * (seq_t)i + (seq_t)2; /* (4*i+2) */
        seq_t a_next = coef * a_curr - a_prev;          /* recurrence */
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
