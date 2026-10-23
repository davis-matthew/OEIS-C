/*
 * A042332  Numerators of continued fraction convergents to sqrt(693).
 * https://oeis.org/A042332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) is the numerator of the n-th convergent in the
 *            continued‑fraction expansion of sqrt(693). The continued
 *            fraction has the first term a0 = 26 and the repeating part
 *            [3,12,1,4,1,12,3,52]. The numerators satisfy a(0) = 26; a(n) =
 *            c_n * a(n-1) + a(n-2) for n ≥ 1, where c_n is the n‑th
 *            coefficient...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* period of the continued fraction coefficient sequence for sqrt(693) */
    static const seq_t period[8] = { (seq_t)3, (seq_t)12, (seq_t)1, (seq_t)4,
                                     (seq_t)1, (seq_t)12, (seq_t)3, (seq_t)52 };
    const idx_t period_len = 8;

    /* recurrence: p_{-2}=0, p_{-1}=1, a0=26 */
    seq_t p_prev2 = 0;          /* p_{k-2} */
    seq_t p_prev1 = 1;          /* p_{k-1} */

    /* Compute p_0 first (k=0) using a0 = 26 */
    seq_t coeff = 26;          /* a0 */
    seq_t p_k   = coeff * p_prev1 + p_prev2;   /* p0 = 26*1 + 0 = 26 */
    p_prev2 = p_prev1;         /* p_{-1} becomes new p_{k-1} */
    p_prev1 = p_k;             /* p_k becomes new p_{k-1} */

    if (n == 0u)
        return p_prev1;        /* already computed as p0 */

    /* For k = 1 .. n compute using periodic coefficients */
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t idx = (k - 1u) % period_len;   /* index into period for coefficient c_k */
        coeff = period[idx];
        p_k = coeff * p_prev1 + p_prev2;     /* recurrence */
        p_prev2 = p_prev1;
        p_prev1 = p_k;
    }

    return p_prev1;
}
