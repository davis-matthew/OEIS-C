/*
 * A041406  Numerators of continued fraction convergents to sqrt(218).
 * https://oeis.org/A041406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the nth convergent to sqrt(218) Recurrence:
 *            a(0)=14, a(n)=c[n]*a(n-1)+a(n-2) for n≥1 coefficients c[n]
 *            repeat the cycle 1,3,3,1,28 (period 5)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Coefficients for the continued fraction of sqrt(218) after the initial term */
    static const seq_t cycle[5] = {1, 3, 3, 1, 28};

    seq_t p_prev_prev = 0;   /* p_{-2} */
    seq_t p_prev     = 1;    /* p_{-1} */
    seq_t p_current  = 0;    /* will hold p_n */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0) {
            ai = 14;                 /* a_0 */
        } else {
            ai = cycle[(i - 1) % 5]; /* coefficients from the cycle */
        }
        p_current = ai * p_prev + p_prev_prev;
        p_prev_prev = p_prev;
        p_prev = p_current;
    }
    return p_current;
}
