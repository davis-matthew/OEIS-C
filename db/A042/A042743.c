/*
 * A042743  Denominators of continued fraction convergents to sqrt(902).
 * https://oeis.org/A042743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for denominators of convergents to sqrt(902). */
/* a(0) = 1, a(1) = 30. For n ≥ 2, a(n) = coeff(n)·a(n‑1) + a(n‑2) where
   coeff(n) = 60 if n is even, 30 if n is odd. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)30;

    seq_t q_prev2 = (seq_t)1;   /* a(0)   */
    seq_t q_prev1 = (seq_t)30;  /* a(1)   */

    seq_t q_curr;
    const seq_t coeff_even = (seq_t)60;
    const seq_t coeff_odd  = (seq_t)30;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t coeff = (i & (idx_t)1) ? coeff_odd : coeff_even;
        q_curr = coeff * q_prev1 + q_prev2;
        q_prev2 = q_prev1;
        q_prev1 = q_curr;
    }

    return q_curr;
}
