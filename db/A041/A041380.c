/*
 * A041380  Numerators of continued fraction convergents to sqrt(205).
 * https://oeis.org/A041380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of numerators of continued‑fraction convergents to sqrt(205).
   The continued fraction of sqrt(205) is [14; overline{3,6,1,4,1,6,3,28}].
   The numerators p_n satisfy
      p_{-2} = 0,  p_{-1} = 1,
      p_n   = a_n * p_{n-1} + p_{n-2},
   where a_0 = 14 and for n ≥ 1, a_n is the (n‑1)-th term of the period
   {3,6,1,4,1,6,3,28}. */

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of sqrt(205) */
    const unsigned int period[8] = {3, 6, 1, 4, 1, 6, 3, 28};

    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p = 0;         /* current numerator */

    seq_t a_term;        /* current a_n */

    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            a_term = (seq_t)14;          /* a_0 */
        } else {
            unsigned int idx = (unsigned int)((i - 1) % 8); /* period index */
            a_term = (seq_t)period[idx];
        }

        /* recurrence p_n = a_n * p_{n-1} + p_{n-2} */
        p = a_term * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    return p;   /* p equals a(n) */
}
