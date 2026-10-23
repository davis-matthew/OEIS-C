/*
 * A042850  Numerators of continued fraction convergents to sqrt(956).
 * https://oeis.org/A042850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 20, u64 n <= 40, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = numerators of the convergents of sqrt(956).
 * Let a0 = floor(sqrt(956)) = 30.
 * The recurrence for the convergents is:
 *   p_-2 = 0,   p_-1 = 1
 *   p_k   = a_k * p_{k-1} + p_{k-2} ,   k ≥ 0
 * where a_k are the partial quotients of the continued fraction of sqrt(956).
 * The partial quotients are obtained by the standard algorithm:
 *   m_0 = 0; d_0 = 1; a0 = floor(sqrt(956))
 *   m_{k+1} = d_k * a_k - m_k
 *   d_{k+1} = (956 - m_{k+1}^2) / d_k
 *   a_{k+1} = floor((a0 + m_{k+1}) / d_{k+1})
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    const unsigned long long D   = 956ULL;
    const unsigned long long A0  = 30ULL;   /* floor sqrt(956) */

    if (n == 0)
        return (seq_t)A0;

    /* Continued fraction variables */
    unsigned long long m = 0;
    unsigned long long d = 1;
    unsigned long long a_cur = A0;

    /* Convergent recurrence */
    seq_t p_prev_prev = (seq_t)1; /* p_-1 */
    seq_t p_prev      = (seq_t)A0;/* p_0 */

    for (idx_t k = 1; k <= n; ++k) {
        /* next partial quotient a_k */
        m = d * a_cur - m;
        d = (D - m * m) / d;
        a_cur = (A0 + m) / d;

        /* compute next numerator */
        seq_t p_curr = (seq_t)a_cur * p_prev + p_prev_prev;
        p_prev_prev = p_prev;
        p_prev      = p_curr;
    }
    return p_prev;
}
