/*
 * A042816  Numerators of continued fraction convergents to sqrt(939).
 * https://oeis.org/A042816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the nth convergent of sqrt(939)
 *            (continued‑fraction coefficients periodic:
 *            1,1,1,4,20,4,1,1,1,60)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const unsigned int period[10] = {1,1,1,4,20,4,1,1,1,60};
    seq_t p_minus2 = 0;
    seq_t p_minus1 = 1;
    seq_t curr;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t coeff;
        if (i == 0) {
            coeff = (seq_t)30;                 /* a0 */
        } else {
            unsigned int idx = (unsigned int)((i - 1) % 10);
            coeff = (seq_t)period[idx];       /* a1.. */
        }
        curr = coeff * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = curr;
    }
    return p_minus1;
}
