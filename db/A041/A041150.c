/*
 * A041150  Numerators of continued fraction convergents to sqrt(85).
 * https://oeis.org/A041150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=9; for n≥1, a(n)=c[(n-1)%5]*a(n-1)+a(n-2) where the
 *            continued‑fraction coefficients for sqrt(85) are c={4,1,1,4,18}
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 15, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const int period[5] = {4, 1, 1, 4, 18};

    seq_t p_prev2 = (seq_t)1;   /* p_{-1} */
    seq_t p_prev1 = (seq_t)9;   /* p_0   */

    for (idx_t k = 1; k <= n; ++k) {
        int coeff = period[(k - 1) % 5];
        seq_t p = (seq_t)coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
