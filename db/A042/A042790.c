/*
 * A042790  Numerators of continued fraction convergents to sqrt(926).
 * https://oeis.org/A042790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 34, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = numerator of the nth convergent of sqrt(926)
   Recurrence: h_{-2}=0, h_{-1}=1
                h_k = a_k·h_{k-1}+h_{k-2},
   where a_0 = floor(sqrt(926)) = 30 and the a_k are the continued‑fraction
   coefficients of sqrt(926).  The implementation memoises the terms
   so that repeated calls are O(1) for already computed indices. */

#include <stdlib.h>

static seq_t *cache = NULL;       /* cache of computed terms */
static idx_t cache_cap = 0;       /* cache capacity */
static idx_t cache_len = 0;       /* number of terms stored (>=1 after init) */

/* continued‑fraction constants for sqrt(926) */
static const idx_t sqrtD = 30;    /* floor(sqrt(926)) */
static const idx_t D     = 926;    /* the radicand */

/* state for the next coefficient a_k */
static idx_t  m_state, d_state, a_state;
static seq_t  prev2_state, prev1_state;  /* h_{k-2} and h_{k-1} */
static int    initialized = 0;          /* first call flag */

/* Compute the nth term */
seq_t a(idx_t n)
{
    /* First time through: initialise cache and state */
    if (!initialized) {
        /* term 0 */
        seq_t term0 = (seq_t)sqrtD;   /* a0 * 1 + 0 */
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = term0;
        cache_len = 1;

        /* recurrence state after term 0 */
        prev2_state = (seq_t)1;       /* h_{-1} */
        prev1_state = term0;          /* h_0 */

        /* continued‑fraction state for a_0 */
        m_state = 0;
        d_state = 1;
        a_state = sqrtD;              /* a_0 */

        /* prepare coefficient for term 1 (k = 1) */
        idx_t m_next = d_state * a_state - m_state;
        idx_t d_next = (D - m_next * m_next) / d_state;
        idx_t a_next = (sqrtD + m_next) / d_next;
        m_state = m_next;
        d_state = d_next;
        a_state = a_next;

        initialized = 1;
    }

    /* Extend cache up to n (if necessary) */
    while (cache_len <= n) {
        /* compute next term using current coefficient a_state */
        seq_t term = ((seq_t)a_state) * prev1_state + prev2_state;

        /* grow cache if needed */
        if (cache_len == cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;    /* allocation failed – unlikely */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = term;

        /* advance recurrence state */
        prev2_state = prev1_state;
        prev1_state = term;

        /* compute coefficient for next term */
        idx_t m_next = d_state * a_state - m_state;
        idx_t d_next = (D - m_next * m_next) / d_state;
        idx_t a_next = (sqrtD + m_next) / d_next;
        m_state = m_next;
        d_state = d_next;
        a_state = a_next;
    }

    return cache[n];
}
