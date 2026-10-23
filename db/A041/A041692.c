/*
 * A041692  Numerators of continued fraction convergents to sqrt(366).
 * https://oeis.org/A041692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=19, a(n)=c[n]*a(n-1)+a(n-2) for n >= 1, where c[n] =
 *            period[(n-1) % 12] and period = {7, 1, 1, 1, 2, 12, 2, 1, 1, 1,
 *            7, 38}
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)19;

    /* previous two numerators: h_{-1}=1, h_0=19 */
    seq_t h_prev2 = (seq_t)1;
    seq_t h_prev1 = (seq_t)19;

    /* coefficient period for sqrt(366) */
    static const unsigned period[12] = {7, 1, 1, 1, 2, 12, 2,
                                        1, 1, 1, 7, 38};

    for (idx_t i = 1; i <= n; ++i)
    {
        unsigned coeff = period[(i - 1) % 12];
        seq_t h_new = (seq_t)coeff * h_prev1 + h_prev2;
        h_prev2 = h_prev1;
        h_prev1 = h_new;
    }
    return h_prev1;
}
