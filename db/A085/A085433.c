/*
 * A085433  Resultant of the polynomial x^3-1 and the Chebyshev polynomial of
 *          the second kind U_n(x).
 * https://oeis.org/A085433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(r_n^2 - r_n*s_n + s_n^2) * * Here r_n + s_n*ω =
 *            U_n(ω), ω being a primitive 3-rd root of unity. * The pair (r_n
 *            , s_n) satisfies the integer recurrence * r_0 = 1, s_0 = 0 * r_1
 *            = 0, s_1 = 2 * r_{k+1} = -2*s_k - r_{k-1} * s_{k+1} = 2*(r_k -
 *            s_k) - s_{k-1} * * The sequence a(n) is the...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1, but define a(0)=1 for safety */
        return 1;

    /* r and s may become negative, so we rely on signed arithmetic
       of type seq_t (the arbitrary‑precision implementation is signed). */
    seq_t r_prev2 = 1;  /* r_0   */
    seq_t s_prev2 = 0;  /* s_0   */
    seq_t r_prev1 = 0;  /* r_1   */
    seq_t s_prev1 = 2;  /* s_1   */
    seq_t r_next, s_next;

    if (n == 1) /* skip loop, compute directly */
        ;
    else
    {
        for (idx_t k = 2; k <= n; ++k)
        {
            /* Recursive step based on the relations above. */
            r_next = -2 * s_prev1 - r_prev2;
            s_next = 2 * (r_prev1 - s_prev1) - s_prev2;

            /* advance the window */
            r_prev2 = r_prev1;   s_prev2 = s_prev1;
            r_prev1 = r_next;    s_prev1 = s_next;
        }
    }

    /* At this point r_prev1 = r_n, s_prev1 = s_n */
    seq_t r = r_prev1;
    seq_t s = s_prev1;

    /* w_n = r^2 - r*s + s^2 */
    seq_t r2 = r * r;
    seq_t s2 = s * s;
    seq_t rs = r * s;
    seq_t w = r2 - rs + s2;

    /* multiply by (n+1) to get the resultant */
    seq_t result = w * ((seq_t)n + 1);
    return result;
}
