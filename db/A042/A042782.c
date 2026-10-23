/*
 * A042782  Numerators of continued fraction convergents to sqrt(922).
 * https://oeis.org/A042782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(922); * a_0 =
 *            30, partial quotients repeat with period 19: *
 *            [2,1,2,1,9,2,1,1,6,6,1,1,2,9,1,2,1,2,60]. * The numerator
 *            sequence satisfies * p_{-2} = 0, p_{-1} = 1, * p_n = a_n *
 *            p_{n-1} + p_{n-2} for n >= 0
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constants for the continued fraction of sqrt(922) */
    const seq_t a0     = (seq_t)30;
    const seq_t period[] = { (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)1,
                             (seq_t)9, (seq_t)2, (seq_t)1, (seq_t)1,
                             (seq_t)6, (seq_t)6, (seq_t)1, (seq_t)1,
                             (seq_t)2, (seq_t)9, (seq_t)1, (seq_t)2,
                             (seq_t)1, (seq_t)2, (seq_t)60 };
    const idx_t period_len = 19UL;

    seq_t p_prev_prev = (seq_t)0;  /* p_{-2} */
    seq_t p_prev      = (seq_t)1;  /* p_{-1} */
    seq_t p_current   = (seq_t)0;

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai;
        if (i == 0)
            ai = a0;
        else
            ai = period[(i - 1) % period_len];

        p_current = ai * p_prev + p_prev_prev;
        p_prev_prev = p_prev;
        p_prev = p_current;
    }

    return p_current;
}
