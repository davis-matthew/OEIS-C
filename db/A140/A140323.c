/*
 * A140323  First differences of A140322.
 * https://oeis.org/A140323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 3*a(n+1) + 4*a(n); a(1)=1, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == 0)         /* offset starts at 1; value for n==0 is irrelevant */
        return 0;
    if (n == 1)
        return 1;
    if (n == 2)
        return 9;

    /* Iterative computation of the linear recurrence */
    seq_t prevprev = 1; /* a(1) */
    seq_t prev     = 9; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = 3 * prev + 4 * prevprev;
        prevprev = prev;
        prev = cur;
    }
    return prev;
}
