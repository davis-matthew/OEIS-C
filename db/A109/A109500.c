/*
 * A109500  Number of closed walks of length n on the complete graph on 6
 *          nodes from a given node.
 * https://oeis.org/A109500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 4*a(n-1) + 5*a(n-2), a(0)=1, a(1)=0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Iteratively compute the recurrence up to n. */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)0;   /* a(1) */
    seq_t a_curr = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 4*a(i-1) + 5*a(i-2) */
        a_curr = ((seq_t)4 * a_prev1) + ((seq_t)5 * a_prev2);

        /* Shift for next iteration. */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
