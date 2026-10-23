/*
 * A042714  Numerators of continued fraction convergents to sqrt(887).
 * https://oeis.org/A042714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of the n-th convergent of the
 *            continued‑fraction expansion of sqrt(887)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  p_{-2} = 0 , p_{-1} = 1 ;
 *              p_n   = a_n * p_{n-1} + p_{n-2}
 *  where a_0 = floor(sqrt(887)) = 29 and the a_i (i ≥ 1) are the regular
 *  continued‑fraction partial quotients of sqrt(887).  The sequence starts
 *  with a(0) = 29, a(1) = 30, a(2) = 119, …  (see OEIS A042714).  */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* constants for sqrt(887) */
    const uint64_t D   = 887ULL;   /* radicand */
    const uint64_t a0  = 29ULL;    /* floor(sqrt(D)) */

    seq_t  p_nm2 = 0;   /* p_{-2} */
    seq_t  p_nm1 = 1;   /* p_{-1} */

    uint64_t m   = 0;   /* m_0   */
    uint64_t d   = 1;   /* d_0   */
    uint64_t a_coeff = a0;   /* a_0   */

    /* Iterate up to index n */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t coeff_seq = (seq_t) a_coeff;           /* a_i as seq_t */

        seq_t p = coeff_seq * p_nm1 + p_nm2;         /* p_i  */
        if (i == n)
            return p;

        /* Compute next partial quotient a_{i+1} and updated m,d */
        uint64_t m_next = d * a_coeff - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* Shift recurrence state */
        p_nm2 = p_nm1;
        p_nm1 = p;

        /* Update continued‑fraction state for next iteration */
        m   = m_next;
        d   = d_next;
        a_coeff = a_next;
    }

    /* Should never reach here */
    return 0;
}
