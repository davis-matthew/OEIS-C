/*
 * A041710  Numerators of continued fraction convergents to sqrt(375).
 * https://oeis.org/A041710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041710: a(0)=19; a(1)=39;
   For n≥2, a(n)=s_n*a(n-1)+a(n-2),
   where the partial quotients s_n are periodic:
   2,1,2,1,5,1,2,1,2,38 */
seq_t a(idx_t n)
{
    /* Handle the trivial case directly. */
    if (n == (idx_t)0u)
        return (seq_t)19;

    /* The periodic part of the continued‑fraction coefficients
       of √375, excluding the first term a0 = 19. */
    static const int period[10] = {
        2, 1, 2, 1, 5, 1, 2, 1, 2, 38
    };

    /* Standard recurrence for convergent numerators: 
       p_{-2} = 0,  p_{-1} = 1,  p_n = a_n * p_{n-1} + p_{n-2}. */
    seq_t p_prev2 = (seq_t)0;          /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;          /* p_{-1} */
    seq_t p_cur   = (seq_t)0;

    for (idx_t i = (idx_t)0u; i <= n; ++i)
    {
        int a_i;
        if (i == (idx_t)0u)
            a_i = 19;                    /* a0 */
        else
            a_i = period[(i - (idx_t)1u) % (idx_t)10u];

        /* Compute the next numerator.  Cast a_i to seq_t
           before multiplication to avoid overflow of the
           intermediate integer type. */
        p_cur = (seq_t)a_i * p_prev1 + p_prev2;

        /* Shift the state for the next iteration. */
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    return p_cur;
}
