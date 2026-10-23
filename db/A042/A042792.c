/*
 * A042792  Numerators of continued fraction convergents to sqrt(927).
 * https://oeis.org/A042792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) is the numerator of the n-th convergent to sqrt(927).
 *            Convergents are computed by the recurrence p_-2 = 0, p_-1 = 1
 *            p_k = a_k * p_{k-1} + p_{k-2} with coefficients a_k: a_0 = 30,
 *            a_1..a_8 = 2, 4, 5, 3, 5, 4, 2, 60 (period 8), and thereafter
 *            a_{k+8}=a_k for k≥1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑computed continued fraction coefficients for sqrt(927) */
    static const seq_t coeffs[8] = {
        (seq_t)2, (seq_t)4, (seq_t)5, (seq_t)3,
        (seq_t)5, (seq_t)4, (seq_t)2, (seq_t)60
    };
    const seq_t a0 = (seq_t)30;

    seq_t p_minus2 = (seq_t)0;   /* p_-2 */
    seq_t p_minus1 = (seq_t)1;   /* p_-1 */
    seq_t curr = (seq_t)0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t coeff = (k == 0) ? a0 : coeffs[(k - 1) % 8];
        curr = coeff * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = curr;
    }
    return curr;
}
