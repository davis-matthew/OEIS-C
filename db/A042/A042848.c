/*
 * A042848  Numerators of continued fraction convergents to sqrt(955).
 * https://oeis.org/A042848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(955). */
/* Recurrence: Let a0 = 30 and the continued‑fraction coefficients after the
 * integer part repeat the period
 *   [1,9,3,6,1,1,5,12,5,1,1,6,3,9,1,60].
 * The numerators p_n satisfy
 *   p_-2 = 0,  p_-1 = 1,
 *   p_n   = a_n·p_{n-1} + p_{n-2}   (n ≥ 0),
 *   where a_0 = 30 and a_n is the (*n*)‑th coefficient described above.
 * a(n) = p_n for all n ≥ 0. */
seq_t a(idx_t n)
{
    static const seq_t period[16] = {
        (seq_t)1, (seq_t)9, (seq_t)3, (seq_t)6,
        (seq_t)1, (seq_t)1, (seq_t)5, (seq_t)12,
        (seq_t)5, (seq_t)1, (seq_t)1, (seq_t)6,
        (seq_t)3, (seq_t)9, (seq_t)1, (seq_t)60
    };
    seq_t p_prev2 = 0, p_prev1 = 1, p, coeff;
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            coeff = (seq_t)30;
        } else {
            coeff = period[(i - 1) & 15];   /* period length = 16 */
        }
        p = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p_prev1;
}
