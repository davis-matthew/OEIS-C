/*
 * A040735  Continued fraction for sqrt(763).
 * https://oeis.org/A040735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040735: continued‑fraction partial quotients for sqrt(763)
 * a(0) = 27
 * for n > 0:
 *   let a0 = floor(sqrt(763)) = 27,
 *   initialize m=0, d=1, a_prev = a0.
 *   repeat:
 *     m = a_prev * d - m;
 *     d = (763 - m*m) / d;
 *     a_curr = (a0 + m) / d;   // floor
 *     a_prev = a_curr;
 *   until n steps are taken.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    const seq_t D  = (seq_t)763;   /* square‑free part of the radicand   */
    const seq_t a0 = (seq_t)27;    /* floor(sqrt(D))                     */

    if (n == 0)
        return a0;                 /* a(0) = 27                         */

    seq_t m       = (seq_t)0;     /* m0                                 */
    seq_t d       = (seq_t)1;     /* d0                                 */
    seq_t a_prev  = a0;           /* a0                                 */
    seq_t a_curr  = (seq_t)0;     /* placeholder                        */

    for (idx_t i = 1; i <= n; ++i) {
        /* update m and d from the previous step */
        m = a_prev * d - m;
        d = (D - m * m) / d;
        /* compute the next partial quotient */
        a_curr = (a0 + m) / d;
        a_prev = a_curr;
        if (i == n)
            return a_curr;
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
