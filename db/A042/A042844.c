/*
 * A042844  Numerators of continued fraction convergents to sqrt(953).
 * https://oeis.org/A042844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=30; a(1)=31; a(n)=c_n*a(n-1)+a(n-2) where c_n are the
 *            continued‑fraction coefficients of sqrt(953):
 *            [30,1,6,1,2,1,3,8,1,1,4,4,1,1,8,3,1,2,1,6,1,60] repeating after
 *            the first term
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Continued fraction coefficients of sqrt(953), including a_0 = 30 */
    static const int cf[22] = {30, 1, 6, 1, 2, 1, 3, 8, 1, 1,
                               4, 4, 1, 1, 8, 3, 1, 2, 1, 6,
                               1, 60};

    /* p_{-2} = 0, p_{-1} = 1 for the recurrence */
    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;
    seq_t p = 0;

    for (idx_t i = 0; i <= n; ++i) {
        int idx;
        if (i < 22) {
            idx = (int)i;                  /* use the stored coefficient directly */
        } else {
            /* After a_0, the period is 21: a_1 … a_21 repeat. */
            idx = (int)(((i - 1) % 21) + 1);
        }
        seq_t coeff = (seq_t)cf[idx];
        p = coeff * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p;
    }

    return p;
}
