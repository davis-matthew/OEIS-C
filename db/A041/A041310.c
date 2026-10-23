/*
 * A041310  Numerators of continued fraction convergents to sqrt(168).
 * https://oeis.org/A041310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, cofr, easy
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(168). a(0)=12;
 *            a(-2)=0; a(-1)=1; a(n)=c(n)*a(n-1)+a(n-2), where c(0)=12 and
 *            subsequent c(n) are the continued fraction terms of sqrt(168)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)12;

    seq_t h_im2 = 0;
    seq_t h_im1 = 1;
    seq_t h_i;
    idx_t m = 0;
    idx_t d = 1;
    const idx_t a0 = 12;          /* floor(sqrt(168)) */
    idx_t a_coeff = a0;           /* current CF term */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* compute numerator for current coefficient */
        h_i = (seq_t)a_coeff * h_im1 + h_im2;

        if (i == n)
            return h_i;

        /* update for next CF term */
        m = d * a_coeff - m;
        d = (168 - m * m) / d;
        a_coeff = (a0 + m) / d;

        /* shift numerators for next iteration */
        h_im2 = h_im1;
        h_im1 = h_i;
    }
    return 0; /* unreachable */
}
