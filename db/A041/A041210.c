/*
 * A041210  Numerators of continued fraction convergents to sqrt(116).
 * https://oeis.org/A041210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of the convergents to sqrt(116)
 * Recurrence:
 *   a(0) = 10
 *   for n ≥ 1: a(n) = c[n-1] * a(n-1) + a(n-2)
 *   where the coefficient sequence c is the periodic block
 *   [1, 3, 2, 1, 4, 1, 2, 3, 1, 20] (period 10) and
 *   the first coefficient a0 = 10 is treated separately.
 */
seq_t a(idx_t n)
{
    /* coefficients for the continued fraction of sqrt(116),
       excluding the initial term 10 */
    static const unsigned short coeff[10] = {
        1, 3, 2, 1, 4, 1, 2, 3, 1, 20
    };

    seq_t h_prev2 = 0;   /* h_{-2} */
    seq_t h_prev1 = 1;   /* h_{-1} */
    seq_t h_curr = 0;    /* h_i */

    unsigned cyc = 0;    /* index into coeff[] */

    for (idx_t i = 0; i <= n; ++i) {
        unsigned short ai;

        if (i == 0) {
            ai = 10;   /* the integer part of sqrt(116) */
        } else {
            ai = coeff[cyc];
            ++cyc;
            if (cyc == 10) cyc = 0;
        }

        h_curr = ((seq_t)ai) * h_prev1 + h_prev2;

        h_prev2 = h_prev1;
        h_prev1 = h_curr;
    }

    return h_curr;
}
