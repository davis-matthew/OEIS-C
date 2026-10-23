/*
 * A042725  Denominators of continued fraction convergents to sqrt(892).
 * https://oeis.org/A042725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: Denominators of continued fraction convergents to sqrt(892).
 * Uses the classic recurrence q[n] = a[n]*q[n-1] + q[n-2] with
 * partial quotients a[0] = 29; thereafter a[1..] is periodic with
 * period 8: {1,6,2,14,2,6,1,58}.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;          /* q0 = 1 */

    /* Periodic part of the continued fraction after a0 */
    static const unsigned period[8] = {1, 6, 2, 14, 2, 6, 1, 58};

    seq_t q_prevprev = 0;                  /* q[-1] = 0 */
    seq_t q_prev     = 1;                  /* q[0]   = 1 */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned ak = period[(k - 1) & 7u]; /* a[k] for k >= 1 */
        seq_t new_q = (seq_t)ak * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev     = new_q;
    }

    return q_prev;                         /* q[n] */
}
