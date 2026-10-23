/*
 * A042052  Numerators of continued fraction convergents to sqrt(550).
 * https://oeis.org/A042052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(550) */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of sqrt(550) */
    static const unsigned period[] = {2,4,1,2,1,1,7,4,7,1,1,2,1,4,2,46};
    const unsigned period_len = sizeof(period)/sizeof(period[0]);

    seq_t prev2 = 0;    /* p_{-2} */
    seq_t prev1 = 1;    /* p_{-1} */
    seq_t cur   = 0;    /* p_k */

    for (idx_t k = 0; k <= n; ++k) {
        unsigned ak;
        if (k == 0)
            ak = 23;                                   /* a_0 */
        else
            ak = period[(unsigned)((k - 1) % period_len)]; /* a_k for k≥1 */

        cur = (seq_t)ak * prev1 + prev2;               /* recurrence */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
