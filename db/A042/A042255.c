/*
 * A042255  Denominators of continued fraction convergents to sqrt(653).
 * https://oeis.org/A042255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; let a_k = 25 if k=0 else period[(k-1)%period_len];
   a(k) = a_k * a(k-1) + a(k-2)  with  a(-1)=0, a(-2)=1. */
/* The array `period` contains the repeating part of the continued fraction of sqrt(653). */

#include <stddef.h>

static unsigned long long period[200];
static size_t period_len = 0;
static unsigned long long a0_val = 0;
static unsigned char init_done = 0;

/* Initialise `period` and `a0_val` once. */
static void init_period(void)
{
    if (init_done) return;

    /* integer part of sqrt(653) */
    a0_val = 25;

    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a = a0_val;
    period_len = 0;

    while (1) {
        m = d * a - m;
        d = (653ULL - m * m) / d;
        a = (a0_val + m) / d;

        if (period_len >= sizeof(period) / sizeof(period[0])) {
            /* safety: stop early if buffer would overflow */
            break;
        }

        period[period_len++] = a;
        if (a == 2ULL * a0_val) break;   /* quadratic surd termination */
    }

    init_done = 1;
}

/* Denominator of the n‑th convergent to sqrt(653). */
seq_t a(idx_t n)
{
    init_period();          /* ensure period is ready */

    if (n == 0) return (seq_t)1;

    seq_t q_prev2 = (seq_t)1;  /* q_{-2} */
    seq_t q_prev1 = (seq_t)0;  /* q_{-1} */
    seq_t q_curr;
    unsigned long long coeff;

    for (idx_t k = 0; k <= n; ++k) {
        if (k == 0)
            coeff = a0_val;
        else
            coeff = period[(size_t)(k - 1) % period_len];

        q_curr = coeff * q_prev1 + q_prev2;
        if (k == n) return q_curr;

        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }

    return (seq_t)0;        /* unreachable, keeps compiler happy */
}
