/*
 * A041738  Numerators of continued fraction convergents to sqrt(389).
 * https://oeis.org/A041738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 49, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041738: Numerators of the continued‑fraction convergents to sqrt(389)
 * The sequence is obtained from the recurrence
 *   p_{-2} = 0 ,  p_{-1} = 1
 *   p_k   = a_k * p_{k-1} + p_{k-2}   for k ≥ 0
 * where a_k are the partial quotients of the continued fraction of sqrt(389)
 * with a_0 = ⌊sqrt(389)⌋ = 19.  This recurrence is evaluated with
 * 64‑bit integer arithmetic for the partial quotients and the required
 * integer operations on the numerators.  The type seq_t may be an
 * arbitrary‑precision integer, and all operations on seq_t are performed
 * element‑wise with the standard arithmetic operators. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constant for the square root */
    const uint64_t D   = 389ULL;          /* D = 389 */
    const uint64_t a0  = 19ULL;           /* floor(sqrt(389)) = 19 */

    /* Handle the first term explicitly */
    if (n == 0)
        return (seq_t)a0;

    /* State variables for the continued fraction of sqrt(D) */
    uint64_t m  = 0ULL;     /* m_0 */
    uint64_t d  = 1ULL;     /* d_0 */
    uint64_t a_k = a0;      /* a_0 */

    /* Convergent numerators: p_{-1} = 1, p_0 = a0 */
    seq_t p_prev2 = (seq_t)1ULL;          /* p_{-1} */
    seq_t p_prev1 = (seq_t)a0;            /* p_0 */

    /* Compute p_n for the requested index */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute next partial quotient a_{k} */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;   /* floor division */

        /* Update the numerator using the recurrence */
        seq_t curr = (seq_t)a_next * p_prev1 + p_prev2;

        /* Shift for the next iteration */
        p_prev2 = p_prev1;
        p_prev1 = curr;

        /* Advance the continued fraction state */
        m   = m_next;
        d   = d_next;
        a_k = a_next;
    }

    return p_prev1;   /* this is p_n = a(n) */
}
