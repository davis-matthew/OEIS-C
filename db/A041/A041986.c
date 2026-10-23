/*
 * A041986  Numerators of continued fraction convergents to sqrt(516).
 * https://oeis.org/A041986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A041986: Numerators of continued‑fraction convergents to sqrt(516).

    Continued fraction of sqrt(516):
        [22; 1,2,1,1,14,1,1,2,1,44] (period 10)

    Let p₋₂ = 0, p₋₁ = 1.
    For n ≥ 0 let a₀ = 22 and for n ≥ 1 let
            aₙ = period[(n‑1) % 10]   // period = {1,2,1,1,14,1,1,2,1,44}
    Then the numerators satisfy the recurrence
            pₙ = aₙ · pₙ₋₁ + pₙ₋₂ .
    The function returns pₙ (with a(0)=p₀=22).
*/
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)22;

    /*  Continued‑fraction coefficients after the integer part 22  */
    static const seq_t period[10] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)1,
        (seq_t)1,
        (seq_t)14,
        (seq_t)1,
        (seq_t)1,
        (seq_t)2,
        (seq_t)1,
        (seq_t)44
    };
    const idx_t period_len = (idx_t)10;

    /*  Initialize recurrence:  p_-1 = 1, p_0 = 22  */
    seq_t p_prev2 = (seq_t)1;       /* p_-1  */
    seq_t p_prev1 = (seq_t)22;      /* p_0   */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ai = period[(i - 1) % period_len];
        seq_t p_curr = ai * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }
    return p_prev1;
}
