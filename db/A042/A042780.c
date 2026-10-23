/*
 * A042780  Numerators of continued fraction convergents to sqrt(921).
 * https://oeis.org/A042780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  p_{-2}=0, p_{-1}=1.
   For k ≥ 0:  p_k = a_k * p_{k-1} + p_{k-2}
   where a_0 = 30 and a_k (k≥1) are the periodic partial
   quotients of the simple continued fraction of sqrt(921). */
/* This function computes the numerator of the nth convergent. */

#include <stdlib.h>

static idx_t *period = NULL;
static size_t period_len = 0;

/* Computes the periodic part of the continued fraction of sqrt(921). */
static void init_period(void)
{
    if (period != NULL)
        return;                      /* already initialized */

    const idx_t D   = 921;           /* the number under the square root */
    const idx_t a0  = 30;            /* floor(sqrt(921)) */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    size_t cap = 16;
    period = (idx_t *)malloc(sizeof(idx_t) * cap);
    period_len = 0;

    /* Perform the standard algorithm until d == 1 (end of period). */
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (period_len >= cap) {
            cap <<= 1;
            period = (idx_t *)realloc(period, sizeof(idx_t) * cap);
        }
        period[period_len++] = a;
    } while (d != 1);
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    init_period();                    /* ensure partial quotients are ready */

    seq_t p_prev2 = 0;                /* p_{-2}  */
    seq_t p_prev1 = 1;                /* p_{-1}  */
    seq_t p_curr;
    seq_t ak;

    for (idx_t k = 0; k <= n; ++k) {
        /* a_k for k = 0 (initial term) and k ≥ 1 (periodic terms). */
        ak = (k == 0) ? (seq_t)30
                      : (seq_t)period[(k - 1) % period_len];

        p_curr = ak * p_prev1 + p_prev2;
        if (k == n)
            return p_curr;           /* requested convergent */

        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    /* Should never reach here; return 0 as a safety guard. */
    return 0;
}
