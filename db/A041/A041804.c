/*
 * A041804  Numerators of continued fraction convergents to sqrt(423).
 * https://oeis.org/A041804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the nth convergent of the continued
 *            fraction of sqrt(423). * The continued fraction of sqrt(423) has
 *            the integer part a0 = 20 and a purely periodic part * of length
 *            8: [1,1,3,4,3,1,1,40]. The recurrence for the numerators p_n is
 *            * p_(-2) = 0, p_(-1) = 1 * p_n = a_n * p_(n...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* periodic part of the continued fraction of sqrt(423) */
    static const unsigned period[8] = {1, 1, 3, 4, 3, 1, 1, 40};
    const idx_t period_len = 8;

    /* recurrence initial values */
    seq_t p_minus2 = 0;   /* p(-2) */
    seq_t p_minus1 = 1;   /* p(-1) */
    seq_t p = 0;          /* current value */

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0) {
            a_i = 20;                           /* a0 */
        } else {
            a_i = period[(i - 1) % period_len]; /* periodic part */
        }
        p = a_i * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = p;
    }
    return p;
}
