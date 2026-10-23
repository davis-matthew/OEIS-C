/*
 * A041138  Numerators of continued fraction convergents to sqrt(78).
 * https://oeis.org/A041138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=8, a(1)=9; for n≥2, a(n) = coeff(n)*a(n‑1)+a(n‑2), where
 *            coeff(n) = [1,4,1,16] repeating (partial quotients of √78)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)9;
    seq_t p_prev2 = (seq_t)8;  /* a(0) */
    seq_t p_prev1 = (seq_t)9;  /* a(1) */
    static const unsigned int period[4] = {1U, 4U, 1U, 16U};
    for (idx_t k = 2; k <= n; ++k) {
        unsigned int coeff = period[(unsigned int)((k-1) % 4)];
        seq_t p_k = (seq_t)coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_k;
    }
    return p_prev1;
}
