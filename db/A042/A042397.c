/*
 * A042397  Denominators of continued fraction convergents to sqrt(725).
 * https://oeis.org/A042397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A042397 – denominators of continued‑fraction convergents to sqrt(725)
 *
 *  Recurrence:
 *      a(0) = 1,  a(1) = 1
 *      a(n) = p[n-1] * a(n-1) + a(n-2)   (n ≥ 2)
 *  where the period of the continued fraction coefficients after the
 *  initial term 26 is  [1,12,2,12,1,52] and starts with p[1] = 1.
 *  This produces the sequence 1, 1, 13, 27, 337, 364, …
 */

#include <stdbool.h>

static const int PERIOD[6] = {1, 12, 2, 12, 1, 52};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t q_prevprev = (seq_t)1; /* a(0) */
    seq_t q_prev     = (seq_t)1; /* a(1) */
    seq_t q_curr;

    for (idx_t i = 2; i <= n; ++i) {
        int period_index = (int)((i - 1) % 6);          /* p[n-1] */
        int p_val = PERIOD[period_index];
        q_curr = (seq_t)p_val * q_prev + q_prevprev;   /* recurrence */
        q_prevprev = q_prev;
        q_prev = q_curr;
    }
    return q_prev;
}
