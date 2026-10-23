/*
 * A042606  Numerators of continued fraction convergents to sqrt(832).
 * https://oeis.org/A042606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(832)
   a(0)=28, a(1)=29,
   a(n) = coeff[(n‑1)%12]*a(n‑1)+a(n‑2),
   coeff = {1,5,2,2,1,13,1,2,2,5,1,56}. */
seq_t a(idx_t n)
{
    /* Periodic continued‑fraction coefficients after the integer part 28. */
    static const seq_t coeff[12] = {
        (seq_t)1, (seq_t)5, (seq_t)2, (seq_t)2,
        (seq_t)1, (seq_t)13, (seq_t)1, (seq_t)2,
        (seq_t)2, (seq_t)5, (seq_t)1, (seq_t)56
    };

    seq_t p_prev2 = (seq_t)1;   /* p_{-1} */
    seq_t p_prev1 = (seq_t)28;  /* p_0 */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t c = coeff[(idx_t)((i - 1) % 12)];
        seq_t p = c * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }

    return p_prev1;
}
