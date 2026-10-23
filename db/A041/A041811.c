/*
 * A041811  Denominators of continued fraction convergents to sqrt(426).
 * https://oeis.org/A041811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A041811: Denominators of continued fraction convergents to sqrt(426)
 *
 *  Computed by the standard recurrence for continued fractions.
 *  The continued fraction of sqrt(426) is
 *
 *      [20; (1,1,1,3,2,6,2,3,1,1,1,40)]  (period length 12)
 *
 *  Using the recurrence
 *
 *      p_k = a_k * p_{k-1} + p_{k-2}
 *      q_k = a_k * q_{k-1} + q_{k-2}
 *
 *  where the first term a_0 = 20 and
 *          a_k = period[(k-1) mod 12] for k >= 1.
 *  The terms q_k are the denominators of the convergents,
 *  i.e. a(k) = q_k.
 *
 *  No external tables are used; the sequence is generated on-the-fly
 *  for any requested index n >= 0.
 *
 *  The function works in any call order; every call recomputes the
 *  convergents up to the requested index.
 */

seq_t a(idx_t n)
{
    /* values of the periodic part after the initial 20 */
    const unsigned pat[12] = {1, 1, 1, 3, 2, 6, 2, 3, 1, 1, 1, 40};

    /* p_{-2} = 0, p_{-1} = 1 ;  q_{-2} = 1, q_{-1} = 0 */
    seq_t p_prevprev = 0, p_prev = 1;
    seq_t q_prevprev = 1, q_prev = 0;

    seq_t result = 0;                /* will hold q_n */

    for (idx_t k = 0; k <= n; ++k) {
        unsigned a_k = (k == 0) ? 20 : pat[(k - 1) % 12];

        /* compute next numerator and denominator */
        seq_t p_curr = (seq_t)a_k * p_prev + p_prevprev;
        seq_t q_curr = (seq_t)a_k * q_prev + q_prevprev;

        if (k == n)
            result = q_curr;        /* store the target term */

        /* shift for next iteration */
        p_prevprev = p_prev;
        p_prev = p_curr;
        q_prevprev = q_prev;
        q_prev = q_curr;
    }

    return result;
}
