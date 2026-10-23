/*
 * A041848  Numerators of continued fraction convergents to sqrt(446).
 * https://oeis.org/A041848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued‑fraction convergents to sqrt(446).  
   The algorithm computes the periodic part of the continued fraction
   of sqrt(446) once, then uses the recurrence
         p(k) = a_k · p(k‑1) + p(k‑2)
   to obtain the k‑th numerator. */

seq_t a(idx_t n)
{
    /* Integer part of sqrt(446). */
    const int a0 = 21;

    /* Periodic part of the continued fraction for sqrt(446). */
    static int period[256];
    static idx_t period_len = 0;
    static int period_computed = 0;

    /* Compute the period once. */
    if (!period_computed) {
        int m = 0, d = 1, a = a0;
        idx_t i = 0;
        while (1) {
            m = d * a - m;
            d = (446 - m * m) / d;
            a = (a0 + m) / d;
            period[i++] = a;
            if (a == 2 * a0)          /* period ends when a == 2*a0 */
                break;
        }
        period_len     = i;
        period_computed = 1;
    }

    /* a(0) is the integer part itself. */
    if (n == 0)
        return (seq_t)a0;

    /* Convergent recurrence.  Keep only the two recent numerators. */
    seq_t p_prev_prev = 1;        /* p(-1) */
    seq_t p_prev      = (seq_t)a0;/* p(0) */
    seq_t p_curr;
    idx_t period_index = 0;

    idx_t k;
    for (k = 1; k <= n; ++k) {
        int coef = period[period_index++];
        if (period_index == period_len)
            period_index = 0;     /* cycle through the periodic part */

        p_curr = (seq_t)coef * p_prev + p_prev_prev;
        p_prev_prev = p_prev;
        p_prev      = p_curr;
    }

    return p_prev;
}
