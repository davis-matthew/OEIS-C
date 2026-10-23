/*
 * A042791  Denominators of continued fraction convergents to sqrt(926).
 * https://oeis.org/A042791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of convergents to √926.
   Recurrence:
      q_{-1} = 0,  q_0 = 1
      q_{k+1} = a_{k+1}·q_k + q_{k-1}
   where the partial quotients a_k are periodic.
   For √D (D = 926) the period ends when a_k = 2·floor(√D). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /*--- compute the period of the continued fraction for √926 ---*/
    const idx_t D   = 926;
    const unsigned a0 = 30;          /* ⌊√926⌋ */
    idx_t m = 0, d = 1;
    unsigned a = a0;

    size_t period_len = 0;
    size_t period_cap = 16;
    int *period = (int*)malloc(period_cap * sizeof(int));
    if (!period) return 0;            /* allocation failure (unlikely) */

    while (1) {
        /* next triple (m, d, a) */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (period_len >= period_cap) {
            period_cap *= 2;
            int *tmp = (int*)realloc(period, period_cap * sizeof(int));
            if (!tmp) { free(period); return 0; }
            period = tmp;
        }
        period[period_len++] = (int)a;

        if (a == 2 * a0) break;     /* end of period */
    }

    /*--- compute the n‑th denominator ---*/
    seq_t qprevprev = (seq_t)0;   /* q_{-1} */
    seq_t qprev     = (seq_t)1;   /* q_0  = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        unsigned ai = (unsigned)period[(i - 1) % period_len];
        seq_t new_q = (seq_t)ai * qprev + qprevprev;
        qprevprev = qprev;
        qprev     = new_q;
    }

    free(period);
    return qprev;                  /* q_n for n ≥ 0 (q_0 already handled) */
}
