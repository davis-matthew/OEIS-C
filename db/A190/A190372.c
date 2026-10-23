/*
 * A190372  a(n) = n + [n*s/r] + [n*t/r] + [n*u/r]; r=sin(Pi/5), s=1/r,
 *          t=sin(2*Pi/5), u=1/t.
 * https://oeis.org/A190372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*(10+2√5)/5⌋ + ⌊n*(1+√5)/2⌋ + ⌊n*(4√5)/5⌋
 * Range:     u8 n <= 35, u16 n <= 8976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fractional_geq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fractional_geq_one.h"

seq_t a(idx_t n)
{
    /* integer parts of the three irrational multipliers */
    const idx_t alpha_int   = 2;   /* floor((10+2√5)/5)   */
    const idx_t beta_int    = 1;   /* floor((1+√5)/2)     */
    const idx_t gamma_int   = 1;   /* floor((4√5)/5)      */

    /* numerators of fractional parts over common denominator 10 */
    const int  alpha_fp_p   = 0;   /* (10+2√5)/5 = 2 + (2/5)√5  -> fractional part (0 + 4√5)/10 */
    const int  alpha_fp_q   = 4;
    const int  beta_fp_p    = -5;  /* (1+√5)/2 = 1 + (√5-1)/2 -> fractional part (-5 + 5√5)/10 */
    const int  beta_fp_q    = 5;
    const int  gamma_fp_p   = -10; /* (4√5)/5 = 1 + (4√5-5)/5 -> fractional part (-10 + 8√5)/10 */
    const int  gamma_fp_q   = 8;

    int64_t p_alpha = 0, q_alpha = 0;   /* fractional part for α */
    int64_t p_beta  = 0, q_beta  = 0;   /* fractional part for φ  */
    int64_t p_gamma = 0, q_gamma = 0;   /* fractional part for γ  */

    idx_t carry_alpha = 0, carry_beta = 0, carry_gamma = 0;

    /* Iterate n times to generate the Beatty sequences */
    for (idx_t i = 0; i < n; ++i)
    {
        /* update for α */
        p_alpha += alpha_fp_p;
        q_alpha += alpha_fp_q;
        while (fractional_geq_one(p_alpha, q_alpha)) {
            p_alpha -= 10;
            ++carry_alpha;
        }

        /* update for φ */
        p_beta  += beta_fp_p;
        q_beta  += beta_fp_q;
        while (fractional_geq_one(p_beta, q_beta)) {
            p_beta -= 10;
            ++carry_beta;
        }

        /* update for γ */
        p_gamma += gamma_fp_p;
        q_gamma += gamma_fp_q;
        while (fractional_geq_one(p_gamma, q_gamma)) {
            p_gamma -= 10;
            ++carry_gamma;
        }
    }

    /* Combine all parts */
    seq_t result = n;
    result += (seq_t)((seq_t)n * alpha_int + (seq_t)carry_alpha);
    result += (seq_t)((seq_t)n * beta_int  + (seq_t)carry_beta);
    result += (seq_t)((seq_t)n * gamma_int + (seq_t)carry_gamma);
    return result;
}
