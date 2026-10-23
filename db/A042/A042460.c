/*
 * A042460  Numerators of continued fraction convergents to sqrt(758).
 * https://oeis.org/A042460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for numerators of continued fraction convergents to sqrt(758).
 *  Let a0 = ⌊√758⌋ = 27.  For n ≥ 1, let b_n be the partial quotients of the
 *  continued fraction (period 18): 1,1,7,2,1,3,3,1,26,1,3,3,1,2,7,1,1,54.
 *  Then the numerators p_n satisfy:
 *        p_{-1} = 1,   p_0 = 27,
 *        p_n   = b_n * p_{n-1} + p_{n-2}   (n ≥ 1).
 *
 *  The function below computes p_n for any non‑negative index n.
 *  It works for the generic `seq_t` type used by the build system.
 */
seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)27;

    /* periodic sequence of partial quotients (length 18) */
    static const unsigned short period[18] = {
        1, 1, 7, 2, 1, 3, 3, 1,
        26, 1, 3, 3, 1, 2, 7, 1, 1, 54
    };

    seq_t p_prevprev = (seq_t)1;   /* p_{-1} */
    seq_t p_prev     = (seq_t)27;  /* p_0   */
    seq_t p_cur;

    /* index into the period array, cycles every 18 steps */
    unsigned int period_idx = 0;

    for (idx_t k = 1; k <= n; ++k) {
        unsigned int ak = period[period_idx];

        /* recurrence: p_k = a_k * p_{k-1} + p_{k-2} */
        p_cur = (seq_t)ak * p_prev + p_prevprev;

        /* shift over for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_cur;

        /* advance period index */
        period_idx++;
        if (period_idx == 18)
            period_idx = 0;
    }

    return p_cur;
}
