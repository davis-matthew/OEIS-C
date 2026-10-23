/*
 * A042864  Numerators of continued fraction convergents to sqrt(964).
 * https://oeis.org/A042864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 31; For n ≥ 1, let a_k be the continued‑fraction partial
 *            quotients of √964, starting with a_0 = 31. Then the numerators
 *            of the convergents satisfy p_{-1} = 1, p_0 = a_0, p_k = a_k *
 *            p_{k-1} + p_{k-2} (k ≥ 1). The function returns p_n, i.e. a(n)
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 18, u64 n <= 36, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: n == 0 gives the first numerator a_0 = 31. */
    if (n == 0)
        return (seq_t)31;

    /* Constants for √964. */
    const uint64_t D   = 964;      /* the radicand (not a perfect square)   */
    const uint64_t a0  = 31;       /* floor(√D)                             */

    /* Variables for the continued‑fraction algorithm:
       m  = m_{k-1},  d  = d_{k-1},  a_k (current partial quotient).       */
    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a_k = a0;             /* a_0 at start                               */

    /* Convergent numerators:  p_{-1} = 1,   p_0 = a0.                     */
    seq_t p_prev2 = (seq_t)1;       /* p_{-1}                                     */
    seq_t p_prev1 = (seq_t)a0;      /* p_0                                        */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute the next m, d and partial quotient a_{k}.                */
        uint64_t m_next = d * a_k - m;
        uint64_t d_next = (D - m_next * m_next) / d;
        uint64_t a_next = (a0 + m_next) / d_next;   /* floor division */

        /* Update the convergent numerator.                                 */
        seq_t p_curr = (seq_t)a_next * p_prev1 + p_prev2;

        /* Shift for the next iteration.                                   */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
        m = m_next;
        d = d_next;
        a_k = a_next;
    }

    /* After the loop, p_prev1 is p_n. */
    return p_prev1;
}
