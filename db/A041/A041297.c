/*
 * A041297  Denominators of continued fraction convergents to sqrt(161).
 * https://oeis.org/A041297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 23550*a(n-10) - a(n-20) * Initial values (a(0)..a(19)): *
 *            1, 1, 3, 13, 16, 45, 61, 289, 639, 928, * 22911, 23839, 70589,
 *            306195, 376784, 1059763, * 1436547, 6805951, 15048449, 21854400
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Denominators of continued‑fraction convergents to sqrt(161).  */
    /* Use the standard recurrence for convergents:
     *   q_{-1} = 0, q_0 = 1
     *   q_k   = a_k * q_{k-1} + q_{k-2}   for k >= 1
     * where the coeffs a_k are produced by the continued fraction
     * expansion of sqrt(161).  */
    const unsigned long long D   = 161ULL;   /* square‑free part */
    const unsigned long long a0  = 12ULL;    /* floor(sqrt(161)) */

    if (n == 0U)
        return (seq_t)1;   /* q_0 = 1 */

    /* Variables for continued fraction generation (small integers). */
    unsigned long long m   = 0ULL;
    unsigned long long d   = 1ULL;
    unsigned long long a_prev = a0;   /* a_0 for first iteration */

    seq_t q_prev2 = (seq_t)0;   /* q_{-1} */
    seq_t q_prev1 = (seq_t)1;   /* q_0   */

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute the next coefficient a_k using the standard algorithm. */
        unsigned long long m_next = d * a_prev - m;
        unsigned long long d_next = (D - m_next * m_next) / d;
        unsigned long long a_k   = (a0 + m_next) / d_next;

        /* Compute q_k = a_k * q_{k-1} + q_{k-2}. */
        seq_t q_k = (seq_t)a_k * q_prev1 + q_prev2;

        /* Update for next iteration. */
        q_prev2 = q_prev1;
        q_prev1 = q_k;
        m = m_next;
        d = d_next;
        a_prev = a_k;
    }

    return q_prev1;   /* q_n */
}
