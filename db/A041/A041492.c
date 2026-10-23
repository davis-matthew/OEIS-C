/*
 * A041492  Numerators of continued fraction convergents to sqrt(263).
 * https://oeis.org/A041492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numerators of continued fraction convergents to sqrt(263).
 *  a(0)=16, a(1)=65, a(2)=81, a(3)=146, ...  (see OEIS A041492)
 *
 *  Recurrence:  
 *      p_{-2}=0, p_{-1}=1
 *      p_n = coeff_n * p_{n-1} + p_{n-2}
 *  where coeff_0 = 16 and, for n≥1, coeff_n repeats with period 12:
 *      4, 1, 1, 1, 1, 15, 1, 1, 1, 1, 4, 32
 */

#include <stdint.h>
#include <stdbool.h>

/* Static storage for the periodic continued‑fraction coefficients */
static bool period_inited = false;
static seq_t period_coeffs[64];
static idx_t period_len = 0;

/* Compute the period of the continued fraction for sqrt(263) once */
static void init_period(void)
{
    if (period_inited) return;

    const idx_t D   = 263;
    const idx_t a0  = 16;          /* floor(sqrt(263)) */
    idx_t m  = 0;
    idx_t d  = 1;
    idx_t a  = a0;
    idx_t a_next;

    do {
        idx_t m_next = d * a - m;
        idx_t d_next = (D - m_next * m_next) / d;
        a_next = (a0 + m_next) / d_next;

        period_coeffs[period_len++] = a_next;

        m = m_next;
        d = d_next;
        a = a_next;
    } while (a_next != 2 * a0);   /* period ends when coefficient = 2*a0 */

    period_inited = true;
}

/* Numerator of the nth convergent to sqrt(263) */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) return (seq_t)16;

    init_period();

    seq_t p_prev2 = (seq_t)0;   /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;   /* p_{-1} */
    seq_t p = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t coeff;
        if (k == 0) {
            coeff = (seq_t)16;                 /* coeff_0 */
        } else {
            idx_t idx = (k - 1) % period_len;  /* get periodic coefficient */
            coeff = period_coeffs[idx];
        }
        p = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p;
    }
    return p;
}
