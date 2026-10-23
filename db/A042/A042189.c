/*
 * A042189  Denominators of continued fraction convergents to sqrt(619).
 * https://oeis.org/A042189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of the nth convergent to sqrt(619) The
 *            convergents satisfy q_{-2} = 1, q_{-1} = 0 q_k = a_k * q_{k-1} +
 *            q_{k-2} for k ≥ 0 where a_0 = 24 and for k ≥ 1 the terms a_k are
 *            the continued‑fraction coefficients of sqrt(619) (period 38).
 *            This function computes the sequence on demand,...
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cached period of the continued fraction expansion of sqrt(619). */
    static bool period_initialized = false;
    static seq_t period[100];     /* a1 … aL (L = 38) */
    static size_t period_len = 0;

    if (!period_initialized)
    {
        /* --- compute the period for sqrt(619) --- */
        const idx_t D  = 619;
        const idx_t a0 = 24;            /* floor(sqrt(619)) */

        idx_t m = 0;                    /* m_0 */
        idx_t d = 1;                    /* d_0 */
        idx_t a = a0;                   /* a_0 */

        /* Generate a1 … aL until a_L = 2*a0, marking the end of the period. */
        do
        {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            period[period_len++] = (seq_t)a;
        } while (a != 2 * a0);

        period_initialized = true;
    }

    /* --- compute the nth denominator q_n --- */
    if (n == 0)
        return 1;        /* q_0 = 1 */

    seq_t q_prevprev = 1;   /* q_{-2} */
    seq_t q_prev     = 0;   /* q_{-1} */
    seq_t q_curr     = 0;
    seq_t a_k;

    for (idx_t i = 0; i <= n; ++i)
    {
        if (i == 0)
            a_k = 24;                         /* a_0 */
        else
            a_k = period[(i - 1) % (idx_t)period_len]; /* a_i for i≥1 */

        q_curr = a_k * q_prev + q_prevprev;
        q_prevprev = q_prev;
        q_prev     = q_curr;
    }

    return q_curr;
}
