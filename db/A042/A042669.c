/*
 * A042669  Denominators of continued fraction convergents to sqrt(864).
 * https://oeis.org/A042669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent of the continued
 *            fraction expansion of sqrt(864). The sequence is computed
 *            directly by the standard convergent recurrence, avoiding any
 *            hard‑coded initial terms
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const int period[12] = { 2, 1, 1, 5, 1, 13, 1, 5, 1, 1, 2, 58 };

seq_t a(idx_t n)
{
    seq_t q_prev2 = 1;   /* q[-2] for the convergent recurrence */
    seq_t q_prev1 = 0;   /* q[-1] */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak = (k == 0 ? 29 :
                     period[(idx_t)((k - 1) % 12)]);   /* partial quotient */
        seq_t qk = ak * q_prev1 + q_prev2;            /* denominator of kth convergent */
        if (k == n) {
            return qk;
        }
        q_prev2 = q_prev1;
        q_prev1 = qk;
    }
    return 0;  /* unreachable, but keeps the compiler happy */
}
