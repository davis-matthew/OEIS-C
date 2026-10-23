/*
 * A041904  Numerators of continued fraction convergents to sqrt(474).
 * https://oeis.org/A041904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   Numerators of continued‑fraction convergents to sqrt(474)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let p_{-2}=0, p_{-1}=1; for i>=0 : p_i = a_i*p_{i-1}+p_{i-2}  */
/* The partial quotients a_i are produced by the standard continued‑fraction */
/* recurrence for sqrt(474), with a_0 = floor(sqrt(474)) = 21.  */
/* The function memoizes all computed numerators so arbitrary order of calls is fine. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* cache of computed numerators   */
    static size_t cache_size = 0;        /* number of terms stored in cache */

    /* Recurrence state for numerators */
    static seq_t prev2 = 0;              /* p_{i-1}                         */
    static seq_t prev1 = 1;              /* p_i                             */

    /* Continued‑fraction state for sqrt(474) */
    static seq_t cur_m = 0;              /* m_i     of current term i      */
    static seq_t cur_d = 1;              /* d_i     of current term i      */
    static seq_t cur_a = 21;             /* a_i     of current term i      */

    /* If value already cached, return immediately. */
    if (cache_size > (size_t)n)
        return cache[n];

    /* Resize cache to hold terms up to n (inclusive). */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {
        new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    }
    if (new_cache == NULL) {
        return (seq_t)0;    /* Out of memory; caller will handle. */
    }
    cache = new_cache;
    size_t start_idx = cache_size;   /* index of first term to compute anew */
    cache_size = new_size;

    /* Compute missing terms from start_idx up to n. */
    for (size_t i = start_idx; i <= (size_t)n; ++i) {
        /* Current numerator p_i = a_i * p_{i-1} + p_{i-2}. */
        seq_t p = cur_a * prev1 + prev2;
        cache[i] = p;

        /* Shift numerators for next iteration. */
        prev2 = prev1;
        prev1 = p;

        /* Compute next continued‑fraction components. */
        seq_t m_next   = cur_d * cur_a - cur_m;
        seq_t temp     = (seq_t)474 - m_next * m_next;
        seq_t d_next   = temp / cur_d;                           /* exact */
        seq_t a_next   = ((seq_t)21 + m_next) / d_next;          /* floor */

        /* Update continued‑fraction state for next term. */
        cur_m = m_next;
        cur_d = d_next;
        cur_a = a_next;
    }

    return cache[n];
}
