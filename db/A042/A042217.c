/*
 * A042217  Denominators of continued fraction convergents to sqrt(634).
 * https://oeis.org/A042217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the n-th convergent to sqrt(634)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int D = 634;
static const int A0 = 25;          /* floor(sqrt(634)) */

static int *period_terms = NULL;   /* array of continued‑fraction terms a_k (k>=1) */
static size_t period_len    = 0;   /* number of terms in the period */
static int period_ready      = 0;   /* whether the period has been computed */

/* compute the period of the continued fraction of sqrt(D) once and store it */
static void init_period(void)
{
    if (period_ready)
        return;

    int m = 0;
    int d = 1;
    int a = A0;

    /* dynamically grow the array of period terms */
    size_t cap = 8;
    period_terms = (int *)malloc(cap * sizeof(int));
    period_len = 0;

    while (1) {
        /* generate the next partial quotient a */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (A0 + m) / d;

        /* store the term */
        if (period_len >= cap) {
            cap <<= 1;
            period_terms = (int *)realloc(period_terms, cap * sizeof(int));
        }
        period_terms[period_len++] = a;

        /* for sqrt(D) the period ends when a == 2*A0 */
        if (a == 2 * A0)
            break;
    }

    period_ready = 1;
}

/* return the denominator of the n-th convergent to sqrt(634) */
seq_t a(idx_t n)
{
    init_period();

    /* recurrence: p_-2 = 0, p_-1 = 1; q_-2 = 1, q_-1 = 0 */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k) {
        int ak_int;
        if (k == 0)
            ak_int = A0;
        else {
            /* the periodic part starts after the first term a0 */
            size_t pos = (size_t)(k - 1) % period_len;
            ak_int = period_terms[pos];
        }

        seq_t ak = (seq_t)ak_int;

        seq_t p = ak * p_prev1 + p_prev2;
        seq_t q = ak * q_prev1 + q_prev2;

        /* shift the recurrence */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q_prev1;
}
