/*
 * A042232  Numerators of continued fraction convergents to sqrt(642).
 * https://oeis.org/A042232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 11554*a(n-6) - a(n-12) (with a(0)=25, a(1)=51, a(2)=76,
 *            a(3)=1875, a(4)=1951, a(5)=5777, a(6)=290801, a(7)=587379,
 *            a(8)=878180, a(9)=21663699, a(10)=22541879, a(11)=66747457).
 *            This sequence gives the numerators of the convergents to
 *            sqrt(642). The code computes the continued‑fraction parti...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* sqrt(642) is not a perfect square, so its continued fraction is
   periodic.  We compute the period once and then generate the
   partial quotients on demand. */
static const idx_t D = 642;
static const idx_t SQRT_D = 25;           /* floor(sqrt(642)) */
static idx_t period_len = 0;
static idx_t period_terms[128] = {0};    /* will be filled lazily */

/* Helper to compute the period of the continued fraction of sqrt(D). */
static void compute_period(void)
{
    if (period_len > 0)   /* already computed */
        return;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = SQRT_D;
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (SQRT_D + m) / d;
        period_terms[period_len++] = a;          /* a_i for i >= 1 */
    } while (a != 2 * SQRT_D);
}

/* Main sequence function.  The recurrence is evaluated by
   rolling through the partial quotients. */
seq_t a(idx_t n)
{
    /* Ensure period is available. */
    compute_period();

    /* Convergent numerator computation.  Use the standard
       recurrence with P[-2] = 0, P[-1] = 1. */
    seq_t p_prev_prev = 0;   /* P[-2] */
    seq_t p_prev      = 1;   /* P[-1] */
    seq_t p_curr      = 0;   /* placeholder */

    for (idx_t i = 0; i <= n; ++i) {
        idx_t an;
        if (i == 0) {
            an = SQRT_D;   /* a[0] */
        } else {
            an = period_terms[(i - 1) % period_len]; /* a[i] for i>0 */
        }
        /* P[n] = a[n] * P[n-1] + P[n-2] */
        p_curr = (seq_t)an * p_prev + p_prev_prev;

        /* Shift for next iteration. */
        p_prev_prev = p_prev;
        p_prev      = p_curr;
    }

    return p_curr;   /* p_curr holds P[n] */
}
