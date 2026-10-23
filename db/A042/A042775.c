/*
 * A042775  Denominators of continued fraction convergents to sqrt(918).
 * https://oeis.org/A042775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Denominators of convergents to sqrt(918).
   a(0) = 1.
   For n ≥ 1:
       a(n) = c_n * a(n-1) + a(n-2),
   where c_n is the n‑th partial quotient of sqrt(918) after the initial
   30 (so c₁ = 3, c₂ = 2, …). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  last_computed = (idx_t)(-1); /* index of last stored value   */

    /* Denominator recurrence state */
    static seq_t q_prevprev = 0; /* a(n‑2)   */
    static seq_t q_prev     = 1; /* a(n‑1)   */

    /* Continued‑fraction state after the first step (after a₀ = 30) */
    static uint64_t m_state = 30; /* m₁   */
    static uint64_t d_state = 18; /* d₁   */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) exit(1);
        cache[0] = (seq_t)1;     /* a(0) = 1 */
        last_computed = 0;
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Compute missing terms up to n */
    while (last_computed < n) {
        /* Next partial quotient c_{last_computed+1} */
        uint64_t c_next = (30 + m_state) / d_state;

        /* Recurrence for denominator */
        seq_t denom_next = ((seq_t)c_next * q_prev) + q_prevprev;

        /* Store result */
        cache[++last_computed] = denom_next;

        /* Shift for next iteration */
        q_prevprev = q_prev;
        q_prev     = denom_next;

        /* Update continued‑fraction state for next coefficient */
        uint64_t m_next = d_state * c_next - m_state;
        uint64_t d_next = (918 - m_next * m_next) / d_state;
        m_state = m_next;
        d_state = d_next;
    }

    return cache[n];
}
