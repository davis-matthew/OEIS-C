/*
 * A210985  Number of segments needed to draw the toothpick structure of
 *          A139250 as it is after 2^n stages.
 * https://oeis.org/A210985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=3, a(n)=5*a(n-1)-4*a(n-2)-4 for n ≥ 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == (idx_t)0)
        return (seq_t)1;
    if (n == (idx_t)1)
        return (seq_t)3;

    /* iterative computation using the recurrence */
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)3; /* a(1) */
    seq_t an;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        /* an = 5*a1 - 4*a0 - 4 */
        an = ((seq_t)5 * a1) - (((seq_t)4 * a0) + (seq_t)4);
        a0 = a1;
        a1 = an;
    }
    return a1;
}
