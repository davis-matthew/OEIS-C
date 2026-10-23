/*
 * A042228  Numerators of continued fraction convergents to sqrt(640).
 * https://oeis.org/A042228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042228: Numerators of continued fraction convergents to sqrt(640) */
#include <stdint.h>

/* Computes the nth term (0‑based) of the sequence. */
seq_t a(idx_t n)
{
    const uint64_t S   = 640ULL;          /* Δ = 640 */
    const uint64_t a0  = 25ULL;           /* floor(sqrt(S)) */

    uint64_t m   = 0;                    /* m_0 */
    uint64_t d   = 1;                    /* d_0 */
    uint64_t a_co = a0;                  /* a_0 */

    seq_t P_minus2 = 0;                  /* P_{-2} */
    seq_t P_minus1 = 1;                  /* P_{-1} */

    for (idx_t i = 0; i <= n; ++i)
    {
        /* numerator of the i‑th convergent */
        seq_t P_i = (seq_t)a_co * P_minus1 + P_minus2;
        if (i == n) return P_i;

        /* prepare next coefficients */
        uint64_t m_next = d * a_co - m;
        uint64_t d_next = (S - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* shift for the next iteration */
        m   = m_next;
        d   = d_next;
        a_co = a_next;

        /* advance convergents */
        P_minus2 = P_minus1;
        P_minus1 = P_i;
    }

    /* never reached */
    return 0;
}
