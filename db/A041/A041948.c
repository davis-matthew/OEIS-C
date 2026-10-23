/*
 * A041948  Numerators of continued fraction convergents to sqrt(497).
 * https://oeis.org/A041948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = p_n where the numerators p_k of the convergents of
 *            sqrt(497) satisfy: p_{-2} = 0, p_{-1} = 1, p_k = coeff_k *
 *            p_{k-1} + p_{k-2}, k ≥ 0 The continued‑fraction coefficients
 *            coeff_k are: coeff_0 = 22, for k ≥ 1 the sequence repeats every
 *            10 terms: 3, 2, 2, 5, 6, 5, 2, 2, 3, 44. This yields th...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the trivial n=0 case naturally in the loop below. */
    static const seq_t period[10] = {
        (seq_t)3, (seq_t)2, (seq_t)2, (seq_t)5,
        (seq_t)6, (seq_t)5, (seq_t)2, (seq_t)2,
        (seq_t)3, (seq_t)44
    };
    seq_t prev2 = (seq_t)0;       /* p_{k-2}  */
    seq_t prev1 = (seq_t)1;       /* p_{k-1}  */
    seq_t curr  = (seq_t)0;       /* p_k      */

    for (idx_t i = (idx_t)0; i <= n; ++i) {
        seq_t coeff;
        if (i == (idx_t)0) {
            coeff = (seq_t)22;
        } else {
            /* period starts after the first coefficient (coeff_0 = 22) */
            idx_t idx = (i - (idx_t)1) % (idx_t)10;
            coeff = period[idx];
        }
        curr = coeff * prev1 + prev2;
        /* shift for the next iteration */
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
