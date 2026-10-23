/*
 * A041634  Numerators of continued fraction convergents to sqrt(336).
 * https://oeis.org/A041634
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(336).
   a(0) = 18
   a(1) = 55
   For n ≥ 2, the (n‑th) numerator p_n is obtained by the standard
   recurrence for continued fractions of √336:

     Let a₀ = ⌊√336⌋ = 18.
     Initialise m₀ = 0, d₀ = 1, a₁ = a₀.
     For each i ≥ 1:
         mᵢ   = dᵢ₋₁ * aᵢ   - mᵢ₋₁
         dᵢ   = (336 - mᵢ²) / dᵢ₋₁
         aᵢ₊₁ = ⌊(a₀ + mᵢ) / dᵢ⌋
     The convergents satisfy   p₋₁ = 1,  p₀ = a₀
     and   pᵢ   = aᵢ * pᵢ₋₁ + pᵢ₋₂  (i ≥ 1).
   The returned value is pₙ, the numerator of the n‑th convergent. */

#include <stdint.h>

seq_t a(idx_t n)
{
    const idx_t D = 336;
    const idx_t a0 = 18;           /* floor(sqrt(336)) */

    if (n == 0)
        return (seq_t)a0;

    /* Continued fraction components */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a_next = a0;

    /* Convergent numerators: p_{-1} = 1, p_{0} = a0 */
    seq_t p_prev2 = (seq_t)1;
    seq_t p_prev1 = (seq_t)a0;
    seq_t p_curr;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* Next CF terms */
        m = d * a_next - m;
        d = (D - m * m) / d;
        a_next = (idx_t)((a0 + m) / d);

        /* Update convergents */
        p_curr = (seq_t)a_next * p_prev1 + p_prev2;

        /* Shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_prev1;
}
