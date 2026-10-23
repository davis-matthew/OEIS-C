/*
 * A041474  Numerators of continued fraction convergents to sqrt(253).
 * https://oeis.org/A041474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(253) (continued
 *            fraction recurrence with coefficients [15;
 *            1,9,1,1,1,2,1,7,4,2,2,2,4,7,1,2,1,1,1,9,1,30])
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 41, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* periodic continued-fraction coefficients of sqrt(253) after the
       initial term 15.  The period length is 22, the last term being 30 (=2*15). */
    static const int period_raw[22] = {
        1, 9, 1, 1, 1, 2, 1, 7, 4, 2,
        2, 2, 4, 7, 1, 2, 1, 1, 1, 9,
        1, 30
    };

    seq_t p_prev_prev = 0;  /* p_{k-2}  */
    seq_t p_prev     = 1;   /* p_{k-1}  */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ai;
        if (k == 0) {
            ai = (seq_t)15;
        } else {
            ai = (seq_t)period_raw[(int)((k - 1) % 22)];
        }

        seq_t p_k = ai * p_prev + p_prev_prev;
        p_prev_prev = p_prev;
        p_prev = p_k;
    }
    return p_prev;
}
