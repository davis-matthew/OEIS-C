/*
 * A041301  Denominators of continued fraction convergents to sqrt(163).
 * https://oeis.org/A041301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued
 *            fraction expansion of sqrt(163). Partial quotients: a0 = 12;
 *            thereafter periodic of length 18:
 *            1,3,3,2,1,1,7,1,11,1,7,1,1,2,3,3,1,24
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the implementation allows, an optional memoization table
       could be added here.  For the required performance a straight
       recurrence suffices. */
    static const int period[19] = {
        12,  /* a0  */
        1, 3, 3, 2, 1, 1, 7, 1, 11, 1,
        7, 1, 1, 2, 3, 3, 1, 24   /* a1 … a18 */
    };

    if (n == 0) return 1;          /* Q_0 = 1 */

    seq_t q_minus2 = 0;            /* Q_{-1} = 0  */
    seq_t q_minus1 = 1;            /* Q_0     = 1  */
    seq_t q_current;

    for (idx_t i = 1; i <= n; ++i) {
        int ai;
        if (i <= 18) {
            ai = period[i];                    /* use stored part */
        } else {
            ai = period[1 + ((i - 1) % 18)];    /* periodic continuation */
        }

        /* Q_i = a_i * Q_{i-1} + Q_{i-2} */
        q_current = (seq_t)ai * q_minus1 + q_minus2;

        /* shift for next iteration */
        q_minus2 = q_minus1;
        q_minus1 = q_current;
    }

    return q_minus1;   /* Q_n  */
}
