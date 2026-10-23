/*
 * A041212  Numerators of continued fraction convergents to sqrt(117).
 * https://oeis.org/A041212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A041212 - Numerators of continued fraction convergents to √117
   Recurrence: a(n) = 1298*a(n-6) - a(n-12).
   Initial 12 terms are calculated from the continued fraction of √117. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* --- compute the first 12 terms from the continued fraction of √117 --- */
    seq_t init[12];
    const seq_t N = (seq_t)117;

    /* integer floor of √N */
    seq_t a0 = 0;
    while ((a0 + 1) * (a0 + 1) <= N) ++a0;

    seq_t m   = 0;
    seq_t d   = 1;
    seq_t prev_a = a0;          /* a_0 */
    seq_t p_m1 = 1;              /* p_{-1} */
    seq_t p_0  = a0;             /* p_0 */
    init[0]    = p_0;

    for (idx_t k = 1; k <= 11; ++k) {
        seq_t m_next  = d * prev_a - m;
        seq_t d_next  = (N - m_next * m_next) / d;
        seq_t curr_a  = (a0 + m_next) / d_next;     /* floor */

        seq_t p_k = curr_a * p_0 + p_m1;
        init[k] = p_k;

        /* shift for the next iteration */
        p_m1 = p_0;
        p_0  = p_k;
        m    = m_next;
        d    = d_next;
        prev_a = curr_a;
    }

    if (n < 12) return init[n];

    /* --- use the linear recurrence for higher indices --- */
    seq_t ring[12];
    for (idx_t i = 0; i < 12; ++i) ring[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        seq_t v = (seq_t)1298 * ring[(i - 6) % 12] - ring[(i - 12) % 12];
        ring[i % 12] = v;
    }

    return ring[n % 12];
}
