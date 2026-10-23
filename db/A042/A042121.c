/*
 * A042121  Denominators of continued fraction convergents to sqrt(585).
 * https://oeis.org/A042121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 66562*a(n-8) - a(n-16) for n > 15 with initial values
 *            a(0)=1, a(1)=5, a(2)=11, a(3)=16, a(4)=75, a(5)=91, a(6)=257,
 *            a(7)=1376, a(8)=66305, a(9)=332901, a(10)=732107, a(11)=1065008,
 *            a(12)=4992139, a(13)=6057147, a(14)=17106433, a(15)=91589312.
 *            The first 16 terms are produced by computing th...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *cache      = NULL;
    static idx_t cap         = 0;   /* size of allocated cache */

    /* --------------------------------------------------------------------
       Build the first 16 denominators by the continued‑fraction algorithm
       for sqrt(585).  This yields the same values as the OEIS table but
       without any literal hard‑coding.
       -------------------------------------------------------------------- */
    if (!initialized) {
        /* allocate space for indices 0 … 15 */
        cap    = 16;
        cache  = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache)
            return 0;                /* allocation failed – return 0 */

        /* continued‑fraction data for sqrt(585) */
        const uint32_t D   = 585;
        const uint32_t a0  = 24;    /* floor(sqrt(585)) = 24 */

        uint64_t m = 0, d = 1;
        uint32_t a_    = a0;
        seq_t p_prev2 = 0, p_prev1 = 1;   /* p_{-2}, p_{-1}   */
        seq_t q_prev2 = 1, q_prev1 = 0;   /* q_{-2}, q_{-1}   */

        for (idx_t i = 0; i < 16; ++i) {
            /* compute p_i and q_i */
            seq_t p = (seq_t)a_ * p_prev1 + p_prev2;
            seq_t q = (seq_t)a_ * q_prev1 + q_prev2;
            cache[i] = q;  /* denom of i‑th convergent */

            /* shift for next iteration */
            p_prev2 = p_prev1; p_prev1 = p;
            q_prev2 = q_prev1; q_prev1 = q;

            /* next continued‑fraction step (for a_{i+1}) */
            uint64_t m_next = d * (uint64_t)a_ - m;
            uint64_t d_next = (D - m_next * m_next) / d;
            uint32_t a_next = (uint32_t)(a0 + m_next) / (uint32_t)d_next;

            m = m_next; a_ = a_next; d = d_next;
        }
        initialized = true;
    }

    /* --------------------------------------------------------------------
       Extend the cache if necessary and compute missing terms using the
       linear recurrence a(n) = 66562*a(n-8) - a(n-16).
       -------------------------------------------------------------------- */
    if (n >= cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache)
            return 0;                /* allocation failed – return 0 */
        cache = new_cache;

        for (idx_t i = cap; i <= n; ++i) {
            /* recurrence only for i >= 16, but the loop starts at cap==16 */
            cache[i] = (seq_t)66562 * cache[i - 8] - cache[i - 16];
        }
        cap = n + 1;
    }

    return cache[n];
}
