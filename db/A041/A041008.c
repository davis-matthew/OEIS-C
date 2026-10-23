/*
 * A041008  Numerators of continued fraction convergents to sqrt(7).
 * https://oeis.org/A041008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 2, a(1) = 3; a(n) = c(n) * a(n-1) + a(n-2) for n ≥ 2,
 *            where the multiplier c(n) repeats [1,1,1,4] cyclically starting
 *            with c(2) = 1. This generates the numerators of the
 *            continued‑fraction convergents to √7
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;
    /* periodic multipliers: 1,1,1,4 */
    const seq_t coeff[4] =
        { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)4 };
    seq_t p_prev2 = (seq_t)2;   /* a(0) */
    seq_t p_prev1 = (seq_t)3;   /* a(1) */
    seq_t p_curr  = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t c = coeff[(i - 1) % 4];
        p_curr = c * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_curr;
}
