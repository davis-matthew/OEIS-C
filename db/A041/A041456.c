/*
 * A041456  Numerators of continued fraction convergents to sqrt(244).
 * https://oeis.org/A041456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Numerators of continued fraction convergents to sqrt(244). The
 *            partial quotients after the integer part are periodic of length
 *            26: {1,1,1,1,1,2,1,5,1,1,9,1,6,1,9,1,1,5,1,2,1,1,1,1,1,30}
 *            Recurrence: p_{-2}=0, p_{-1}=1, p_n = a_n * p_{n-1} + p_{n-2},
 *            a_0=15, a_n as above for n>0
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[26] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)2, (seq_t)1, (seq_t)5, (seq_t)1, (seq_t)1,
        (seq_t)9, (seq_t)1, (seq_t)6, (seq_t)1, (seq_t)9,
        (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)1, (seq_t)2,
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)30
    };

    seq_t p_prevprev = (seq_t)0;   /* p_{-2} */
    seq_t p_prev     = (seq_t)1;   /* p_{-1} */
    seq_t p          = (seq_t)0;

    for (idx_t i = (idx_t)0; i <= n; ++i)
    {
        seq_t a_i;
        if (i == (idx_t)0)
            a_i = (seq_t)15;            /* integer part of sqrt(244) */
        else
            a_i = period[(i - (idx_t)1) % (idx_t)26];

        p = a_i * p_prev + p_prevprev;

        p_prevprev = p_prev;
        p_prev     = p;
    }

    return p_prev;
}
