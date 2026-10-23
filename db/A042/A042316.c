/*
 * A042316  Numerators of continued fraction convergents to sqrt(685).
 * https://oeis.org/A042316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A042316: Numerators of convergents to √685.
 * This function computes the nth numerator using the standard continued‑fraction
 * recurrence for √685.  The recurrence is
 *   p_{-2} = 0,  p_{-1} = 1,
 *   p_k   = a_k · p_{k-1} + p_{k-2},
 * where a_k are the partial quotients of the continued fraction of √685.
 * The partial quotients are generated with the usual recurrence
 *   m_{k+1} = d_k · a_k − m_k,
 *   d_{k+1} = (D − m_{k+1}²) / d_k,
 *   a_{k+1} = (a₀ + m_{k+1}) / d_{k+1},
 * with D = 685 and a₀ = ⌊√685⌋ = 26.
 */
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    const idx_t D   = 685;      /* Radicand */
    const idx_t a0  = 26;       /* floor(sqrt(685)) */

    /* Initial state of the continued‑fraction algorithm */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Convergent numerators: p_{-2}=0, p_{-1}=1 */
    seq_t p_im2 = 0;   /* p_{k-2}  */
    seq_t p_im1 = 1;   /* p_{k-1}  */
    seq_t p     = 0;   /* current numerator */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute current numerator p_k = a_k * p_{k-1} + p_{k-2} */
        p = mul_small(p_im1, (unsigned)a) + p_im2;
        if (k == n)
            return p;

        /* Prepare next partial quotient a_{k+1} */
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;

        /* Shift for next iteration */
        p_im2 = p_im1;
        p_im1 = p;
    }
    /* Unreachable */
    return 0;
}
