/*
 * A042244  Numerators of continued fraction convergents to sqrt(648).
 * https://oeis.org/A042244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of nth convergent of the continued fraction of
 *            sqrt(648)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Memoised cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Extend cache if necessary */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_size) {
        seq_t *new_cache = (seq_t *)calloc(needed, sizeof(seq_t));
        if (!new_cache) return (seq_t)0;                    /* out‑of‑memory */
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }
        cache      = new_cache;
        cache_size = needed;
    }

    /* If cache already contains a(n), return it */
    if (n < cache_size && cache[n] != 0) return cache[n];

    /* Recompute all terms up to n from scratch */
    /* Continued‑fraction data for sqrt(648) */
    const idx_t N   = 648;          /* N = 648 */
    const idx_t a0  = 25;           /* floor(sqrt(648)) */
    idx_t   m = 0;
    idx_t   d = 1;
    idx_t   a = a0;

    /* Convergent recurrence: p_{-2}=0, p_{-1}=1 */
    seq_t p_prev2 = 0;   /* p_{-2} */
    seq_t p_prev1 = 1;   /* p_{-1} */
    seq_t p_curr  = 0;

    /* Compute terms 0 .. n */
    for (size_t i = 0; i <= n; ++i) {
        /* Current a_i */
        if (i > 0) {
            /* Update m, d, a to next continued‑fraction term */
            idx_t m_next = d * a - m;
            idx_t d_next = (N - m_next * m_next) / d;
            idx_t a_next = (a0 + m_next) / d_next;
            m = m_next; d = d_next; a = a_next;
        }

        /* p_i = a_i * p_{i-1} + p_{i-2} */
        p_curr = (seq_t)a * p_prev1 + p_prev2;
        cache[i] = p_curr;

        /* Shift for next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return cache[n];
}
