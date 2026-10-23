/*
 * A042935  Denominators of continued fraction convergents to sqrt(999).
 * https://oeis.org/A042935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1.  For n≥2,
   let t = period[(n-1)%period_len], where period is the
   period of the continued fraction expansion of √999.
   Then a(n) = t·a(n‑1) + a(n‑2).  This yields the denominators
   of the convergents to √999.  The period is computed once
   by the standard continued‑fraction algorithm. */

#include <stdlib.h>
#include <stdbool.h>

/* Number of terms in the period of √999 (known to be < 512). */
#define PERIOD_MAX 512

/* One‑time storage of the period terms. */
static int  period[PERIOD_MAX];
static size_t period_len = 0;
static bool   period_ready = false;

/* Compute the period of √999 once, using only integer arithmetic. */
static void init_period(void)
{
    if (period_ready) return;

    const unsigned long long D = 999ULL;    /* D = 999 */
    const unsigned int a0   = 31;           /* floor(√999) */

    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned int a = a0;

    size_t idx = 0;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period[idx++] = a;
    } while (a != 2 * a0 && idx < PERIOD_MAX);

    period_len   = idx;
    period_ready = true;
}

/* Return the n‑th denominator of the continued fraction convergents to √999. */
seq_t a(idx_t n)
{
    /* Ensure the period is ready. */
    if (!period_ready) init_period();

    /* p_{-2}=0, p_{-1}=1 ;  q_{-2}=1, q_{-1}=0   (q = denominator) */
    seq_t p_prev2 = (seq_t)0;
    seq_t p_prev1 = (seq_t)1;
    seq_t q_prev2 = (seq_t)1;
    seq_t q_prev1 = (seq_t)0;

    seq_t p = p_prev1, q = q_prev1;   /* will hold the current convergent */

    for (idx_t k = 0; k <= n; ++k) {
        unsigned int a_k = (k == 0) ? (unsigned int)31
                                    : (unsigned int)period[(k - 1) % period_len];

        /* Compute next convergent: p_k = a_k * p_{k-1} + p_{k-2} */
        /*                               q_k = a_k * q_{k-1} + q_{k-2} */
        p = a_k * p_prev1 + p_prev2;
        q = a_k * q_prev1 + q_prev2;

        /* Advance previous terms for next iteration. */
        p_prev2 = p_prev1;
        p_prev1 = p;
        q_prev2 = q_prev1;
        q_prev1 = q;
    }

    return q;   /* after loop, q is q_n (denominator) */
}
