/*
 * A042291  Denominators of continued fraction convergents to sqrt(671).
 * https://oeis.org/A042291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(671); a(0) =
 *            1, a(1) = 1, a(2) = 10, a(3) = 21,
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const unsigned int period[10] = {1,9,2,1,1,1,2,9,1,50};

seq_t a(idx_t n)
{
    /* q_{-2} = 1, q_{-1} = 0 */
    seq_t q_prev_prev = (seq_t)1;
    seq_t q_prev      = (seq_t)0;
    seq_t q_curr      = 0;

    for (idx_t i = 0; i <= n; ++i) {
        unsigned int a_val;
        if (i == 0)
            a_val = 25;                      /* a0 */
        else
            a_val = period[(i - 1) % 10];     /* periodic part */

        /* use 128‑bit intermediate if available to avoid overflow */
        q_curr = (seq_t)((unsigned __int128)a_val * q_prev + q_prev_prev);

        q_prev_prev = q_prev;
        q_prev      = q_curr;
    }

    return q_curr;
}
