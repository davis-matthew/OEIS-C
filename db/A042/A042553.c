/*
 * A042553  Denominators of continued fraction convergents to sqrt(805).
 * https://oeis.org/A042553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = denominator of the nth convergent to sqrt(805).
 * The continued fraction of sqrt(805) has period 18, with the partial
 * quotients (excluding the integer part a₀=28) being:
 *   [2,1,2,5,1,13,2,1,10,1,2,13,1,5,2,1,2,56]
 * The recurrence for the denominators is:
 *   q₀ = 1, q₁ = 2
 *   qₙ = aₙ * qₙ₋₁ + qₙ₋₂   for n ≥ 2
 * where aₙ = partial[(n‑1) % 18].
 */

static const seq_t partial[] = {
    2, 1, 2, 5, 1, 13, 2, 1,
    10, 1, 2, 13, 1, 5, 2, 1, 2, 56
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 2;

    seq_t q_prev2 = 1; /* q₀   */
    seq_t q_prev1 = 2; /* q₁   */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t ai = partial[(i - 1) % 18];
        seq_t q_cur = ai * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_cur;
    }
    return q_prev1;
}
