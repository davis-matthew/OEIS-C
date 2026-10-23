/*
 * A042697  Denominators of continued fraction convergents to sqrt(878).
 * https://oeis.org/A042697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n‑th convergent of sqrt(878).
 *            Continued fraction of sqrt(878) has period 16: a0 = 29, period =
 *            [1,1,1,2,2,4,1,28,1,4,2,2,1,1,1,58]. Denominator recurrence:
 *            q[-2] = 1, q[-1] = 0, q_i = a_i * q_{i-1} + q_{i-2} (i ≥ 0)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Partial quotients after the initial term. */
    static const seq_t period[16] = {
        1, 1, 1, 2,
        2, 4, 1, 28,
        1, 4, 2, 2,
        1, 1, 1, 58
    };

    seq_t q_prev2 = 1;   /* q_{-2} */
    seq_t q_prev1 = 0;   /* q_{-1} */
    seq_t q_curr;
    seq_t a_num;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        if (i == 0)
            a_num = (seq_t)29;              /* a0 */
        else
            a_num = period[(i - 1) % 16];   /* a_i for i ≥ 1 */

        /* q_i = a_i * q_{i-1} + q_{i-2} */
        q_curr = a_num * q_prev1 + q_prev2;

        if (i == n)
            return q_curr;

        /* shift for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }

    /* Should never reach here */
    return (seq_t)0;
}
