/*
 * A097828  Partial sums of Chebyshev sequence S(n,13)= U(n,13/2) =
 *          A078362(n).
 * https://oeis.org/A097828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n‑1) - a(n‑2) + 1, a(-1)=0, a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence directly computes the partial sums of the Chebyshev sequence U(n,13/2). */

seq_t a(idx_t n)
{
    /* Handle the base case immediately. */
    if (n == 0)
        return (seq_t)1;

    /* a(-1) = 0, a(0) = 1 */
    seq_t a_prev2 = (seq_t)0;        /* a(k-2) */
    seq_t a_prev1 = (seq_t)1;        /* a(k-1) */

    /* Iterate the recurrence up to the requested n. */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t cur = (seq_t)13 * a_prev1 - a_prev2 + (seq_t)1;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return a_prev1;
}
