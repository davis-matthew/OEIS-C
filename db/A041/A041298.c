/*
 * A041298  Numerators of continued fraction convergents to sqrt(162).
 * https://oeis.org/A041298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   a(0) = 12, a(-1) = 1. For n ≥ 1: a(n) = period[n‑1] * a(n‑1) +
 *            a(n‑2), where period = {1,2,1,2,12,2,1,2,1,24} repeats each 10
 *            terms
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)12;

    seq_t p_prevprev = (seq_t)1;   /* a(-1) */
    seq_t p_prev     = (seq_t)12;  /* a(0) */

    static const seq_t period[10] = {1, 2, 1, 2, 12, 2, 1, 2, 1, 24};

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ai   = period[(i - 1) % 10];
        seq_t cur  = ai * p_prev + p_prevprev;
        p_prevprev = p_prev;
        p_prev     = cur;
    }

    return p_prev;
}
