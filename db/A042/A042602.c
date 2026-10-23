/*
 * A042602  Numerators of continued fraction convergents to sqrt(830).
 * https://oeis.org/A042602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 28
   a(1) = 29
   a(n) = cf(n) * a(n-1) + a(n-2)   (n ≥ 2)
   where cf(n) are the partial quotients of the continued fraction of √830
   (cf[0] = 28, cf[1] = 1, cf[2] = 4, …).  This provides the
   numerators of the convergents to √830.  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* constant part of the continued fraction of sqrt(830) */
    const seq_t N   = 830;   /* N = 830 */
    const seq_t a0  = 28;    /* floor(sqrt(830)) */

    /* trivial case: first numerator */
    if (n == 0) return a0;

    /* variables for generating the continued‑fraction partial quotients */
    seq_t m      = 0;   /* m_0 */
    seq_t d      = 1;   /* d_0 */
    seq_t cf_k   = a0;  /* cf[0] */

    /* recurrence variables for numerators of convergents */
    seq_t p_prev2 = 1;   /* p[-1] */
    seq_t p_prev1 = a0;  /* p[0] */

    /* compute successive convergents up to n */
    for (idx_t k = 1; k <= n; ++k) {
        /* next partial quotient cf[k] */
        m   = d * cf_k - m;
        d   = (N - m * m) / d;
        cf_k = (a0 + m) / d;

        /* next numerator */
        seq_t p_new = cf_k * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_new;
    }

    return p_prev1;
}
