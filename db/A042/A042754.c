/*
 * A042754  Numerators of continued fraction convergents to sqrt(908).
 * https://oeis.org/A042754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent to sqrt(908). The values
 *            satisfy the recurrence p_n = a_n * p_{n-1} + p_{n-2}, where a_0
 *            = 30 and the sequence (a_n)_{n≥1} is the continued fraction
 *            expansion of sqrt(908), which repeats with period 8:
 *            7,1,1,14,1,1,7,60
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_PERIOD 200

static int initialized = 0;
static idx_t period_len;
static seq_t coeffs[MAX_PERIOD + 1];

seq_t a(idx_t n)
{
    /* compute continued fraction coefficients for sqrt(908) once */
    if (!initialized) {
        /* floor sqrt(908) = 30 */
        idx_t a0 = 30;

        /* initial values for the standard algorithm */
        idx_t m = 0;
        idx_t d = 1;
        idx_t a = a0;

        period_len = 0;
        coeffs[0] = (seq_t)a0;          /* a_0 */

        do {
            /* next terms of the continued fraction */
            m = d * a - m;
            d = (908 - m * m) / d;
            a = (a0 + m) / d;

            ++period_len;
            coeffs[period_len] = (seq_t)a;  /* store a_k */
        } while (a != 2 * a0);        /* terminate when the period completes */

        initialized = 1;
    }

    /* recurrence for convergents */
    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p_cur   = 0;

    for (idx_t i = 0; i <= n; ++i) {
        idx_t j = (i == 0) ? 0 : ((i - 1) % period_len + 1);
        seq_t a_i = coeffs[j];
        p_cur = a_i * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_cur;
    }

    return p_cur;
}
