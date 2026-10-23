/*
 * A042335  Denominators of continued fraction convergents to sqrt(694).
 * https://oeis.org/A042335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of sqrt(694) continued
 *            fraction Recurrence: a(0)=1, a(1)=2, a(n)=a_{n} * a(n-1)+a(n-2)
 *            with a_{n} from the continued fraction of sqrt(694)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 21, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constants for the continued fraction of sqrt(694) */
    const seq_t D  = (seq_t)694;
    const seq_t a0 = (seq_t)26;   /* floor(sqrt(694)) */

    if (n == 0) return (seq_t)1;   /* q_0 = 1 */

    /* Continued fraction variables */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a_prev = a0;    /* a_0 */

    /* Convergent denominator recurrence */
    seq_t q_prev2 = 0;    /* q_{-1} */
    seq_t q_prev1 = 1;    /* q_0 */
    seq_t q = 0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Produce next partial quotient a = a_k */
        m = d * a_prev - m;
        d = (D - m * m) / d;
        seq_t a = (a0 + m) / d;

        /* Update convergent denominator */
        q = a * q_prev1 + q_prev2;

        /* Shift for next iteration */
        q_prev2 = q_prev1;
        q_prev1 = q;
        a_prev = a;
    }

    return q_prev1;
}
