/*
 * A041932  Numerators of continued fraction convergents to sqrt(489).
 * https://oeis.org/A041932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(489)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The convergents are defined by the continued fraction of sqrt(489) */
#include <stdint.h>

/* Continuants for sqrt(489) */
seq_t a(idx_t n)
{
    /* Special case for the zeroth convergent */
    if (n == 0)
        return (seq_t)22;            /* a0 = floor(sqrt(489)) */

    /* prev2 = p_{k-2}, prev1 = p_{k-1} */
    seq_t prev2 = (seq_t)0;          /* p_{-2} */
    seq_t prev1 = (seq_t)1;          /* p_{-1} */

    /* First convergent p0 */
    seq_t curr = (seq_t)22 * prev1 + prev2;   /* p0 = a0 * p_{-1} + p_{-2} */
    /* Prepare for next step */
    prev2 = prev1;
    prev1 = curr;

    /* Continued‑fraction data for sqrt(489) */
    const uint64_t D   = 489ULL;     /* discriminant */
    const uint64_t a0  = 22ULL;      /* floor(sqrt(489)) */
    uint64_t m = 0ULL;              /* m_0   */
    uint64_t d = 1ULL;              /* d_0   */
    uint64_t a_cur = a0;            /* a_0   */

    /* Compute convergents up to n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Next term of the continued fraction */
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (a0 + m) / d;

        /* Continuant recurrence: p_k = a_cur * p_{k-1} + p_{k-2} */
        seq_t new_curr = (seq_t)a_cur * prev1 + prev2;

        /* Update for next iteration */
        prev2 = prev1;
        prev1 = new_curr;

        /* If we reached the desired index, return */
        if (k == n)
            return new_curr;
    }

    /* In theory, the loop always returns before reaching this point */
    return curr;
}
