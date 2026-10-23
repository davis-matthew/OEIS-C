/*
 * A042652  Numerators of continued fraction convergents to sqrt(856).
 * https://oeis.org/A042652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let p_{-1}=1, p_0=29.  For n≥1 compute the partial
 * quotient a_n of the continued fraction of √856 by the standard
 * recurrence (m_k, d_k).  Then compute p_n = a_n·p_{n-1} + p_{n-2}.
 * The returned value is the numerator of the n‑th convergent of √856. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)29;          /* a_0 = floor(sqrt(856)) = 29 */

    const uint64_t D  = 856;
    const uint64_t A0 = 29;       /* a_0 */

    /* Continued fraction variables */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_cur = A0;          /* the a_k we are about to compute */

    /* Convergent numerator variables */
    seq_t p_prev2 = (seq_t)1;     /* p_{-1} */
    seq_t p_prev1 = (seq_t)A0;    /* p_0 */
    seq_t p_curr = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* compute next partial quotient a_{i} */
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (A0 + m) / d;

        /* compute p_i from recurrence */
        p_curr = (seq_t)a_cur * p_prev1 + p_prev2;

        /* shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;
}
