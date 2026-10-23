/*
 * A041390  Numerators of continued fraction convergents to sqrt(210).
 * https://oeis.org/A041390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of the continued fraction
 *            of sqrt(210)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: p(-2)=0, p(-1)=1, p(k)=a_k * p(k-1)+p(k-2) where a_k are the CF terms of sqrt(210). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* special case n=0 */
    if (n == 0) return (seq_t)14;   /* a(0)=14 */

    /* continued fraction parameters for sqrt(210) */
    const uint64_t D   = 210;
    const uint64_t a0  = 14;  /* floor(sqrt(210)) */

    /* recurrence for numerators */
    seq_t p_prev2 = 0;  /* p(-2) */
    seq_t p_prev1 = 1;  /* p(-1) */
    seq_t p_curr;

    /* parameters for generating CF terms */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;   /* a_0 */

    for (idx_t k = 0; k <= n; ++k) {
        /* compute p_k */
        p_curr = (seq_t)a * p_prev1 + p_prev2;
        if (k == n) return p_curr;

        /* advance to next CF term */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* shift previous numerators */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    /* control should never reach here */
    return p_curr;
}
