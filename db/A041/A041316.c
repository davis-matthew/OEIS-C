/*
 * A041316  Numerators of continued fraction convergents to sqrt(172).
 * https://oeis.org/A041316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = Numerators of continued fraction convergents to sqrt(172)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 16, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the nth term of OEIS sequence A041316. */
seq_t a(idx_t n)
{
    /* Parameters for √172 */
    const uint64_t D   = 172ULL;           /* target number */
    const uint64_t a0  = 13ULL;            /* floor(sqrt(D)) */

    /* Continued fraction variables */
    uint64_t m = 0ULL;   /* m0 */
    uint64_t d = 1ULL;   /* d0 */
    uint64_t a = a0;     /* a0 */

    /* Convergence recurrence variables */
    seq_t p_prevprev = 0ULL; /* p_{-2} */
    seq_t p_prev     = 1ULL; /* p_{-1} */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute p_k = a_k * p_{k-1} + p_{k-2} */
        seq_t p_k = (seq_t)a * p_prev + p_prevprev;

        if (k == n)
            return p_k;   /* result found */

        /* Prepare for next iteration */
        p_prevprev = p_prev;
        p_prev     = p_k;

        /* Update CF state to obtain next a_{k+1} */
        uint64_t m_next = d * a - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }

    /* This point should never be reached. */
    return 0ULL;
}
