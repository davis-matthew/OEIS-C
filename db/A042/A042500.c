/*
 * A042500  Numerators of continued fraction convergents to sqrt(778).
 * https://oeis.org/A042500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042500
 *  Numerators of continued‑fraction convergents to sqrt(778).
 *  The sequence satisfies the recurrence
 *
 *      p(-1) = 1,  p(0) = ⌊√778⌋ = 27
 *      p(n)  = a(n)·p(n‑1) + p(n‑2)   for n ≥ 1
 *
 *  where a(1), a(2), … are the periodic continued‑fraction coefficients
 *  of √778 starting after the first term.  The period ends when the
 *  coefficient equals 2·⌊√778⌋.  This implementation pre‑computes the
 *  period once and then uses the recurrence to produce any term.
 */

#include <stdbool.h>

#define CF_BUF_SIZE 200          /* ample buffer for the period */

static const idx_t D  = 778;  /* radicand */
static const idx_t A0 = 27;   /* floor(sqrt(D)) */

/* period storage */
static bool        period_inited = false;
static seq_t       cf[CF_BUF_SIZE];   /* cf[0] = A0, cf[1..period_len] = period */
static idx_t       period_len;        /* number of periodic terms */

/* compute the continued‑fraction period of sqrt(D) once */
static void init_period(void)
{
    if (period_inited) return;

    idx_t m = 0, d = 1, a = A0;
    cf[0] = (seq_t)A0;
    period_len = 0;

    while (true) {
        /* next values of the continued‑fraction algorithm */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (A0 + m_next) / d_next;

        period_len++;                     /* store this a_next */
        if (period_len >= CF_BUF_SIZE) {  /* safety guard, should never trigger */
            period_inited = true;
            return;
        }
        cf[period_len] = (seq_t)a_next;

        if (a_next == 2 * A0) {  /* period terminates */
            period_inited = true;
            return;
        }

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

/*  Return the n‑th numerator of the convergents to sqrt(778).   */
seq_t a(idx_t n)
{
    init_period();             /* ensure cf is ready */

    if (n == 0) return cf[0];   /* first term, 27 */

    seq_t p_prev2 = (seq_t)1;   /* p(-1) */
    seq_t p_prev1 = cf[0];      /* p(0)   */
    seq_t p_curr, a_i;

    for (idx_t i = 1; i <= n; ++i) {
        /* coefficient a(i) is cf[(i-1) % period_len + 1] */
        a_i = cf[((i - 1) % period_len) + 1];

        p_curr = a_i * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;
}
