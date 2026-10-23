/*
 * A041245  Denominators of continued fraction convergents to sqrt(134).
 * https://oeis.org/A041245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued fraction convergents to sqrt(134) */

#include <stdint.h>

/* Last floor coefficient: floor(sqrt(134)) = 11
   Period of the continued fraction: 14
   Periodic part: 1, 1, 2, 1, 3, 1, 10, 1, 3, 1, 2, 1, 1, 22 */
#define PERIOD 14
static const int cyc[PERIOD] = {
    1, 1, 2, 1, 3, 1, 10, 1, 3, 1, 2, 1, 1, 22
};

seq_t a(idx_t n)
{
    /* Recurrence for convergents of a quadratic irrational:
         q_{-2} = 1, q_{-1} = 0
         q_k    = a_k * q_{k-1} + q_{k-2}
       where a_0 = floor(sqrt(134)) = 11 and a_k for k≥1 is given by cyc. */
    seq_t q_prev2 = (seq_t)1;   /* q_{-2} */
    seq_t q_prev1 = (seq_t)0;   /* q_{-1} */
    seq_t q = (seq_t)0;         /* current q_k */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t coef;
        if (k == 0) {
            coef = (seq_t)11;
        } else {
            /* cyc[(k-1)%PERIOD] gives a_k for k>=1 */
            int idx = (int)((k - 1) % PERIOD);
            coef = (seq_t)cyc[idx];
        }
        q = coef * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }
    return q;
}
