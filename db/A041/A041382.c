/*
 * A041382  Numerators of continued fraction convergents to sqrt(206).
 * https://oeis.org/A041382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = numerator of the n‑th convergent of the continued
 *            fraction for sqrt(206). Recurrence: let the partial quotients be
 *            a0 = 14, and for k ≥ 1 a_k computed from the standard
 *            continued‑fraction recurrence with D = 206. Then p_{-1} = 1, p_0
 *            = a0, and for k ≥ 1 p_k = a_k * p_{k-1} + p_{k-2}. The f...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 192 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Cached values and state needed for the continued‑fraction generation */
static seq_t *cache        = NULL;      /* array of numerators p_k */
static size_t cache_cap    = 0;         /* allocated capacity of cache   */
static size_t cache_size   = 0;         /* number of stored terms        */
static seq_t  m_curr;          /* m (state) for next a_k         */
static seq_t  d_curr;          /* d (state) for next a_k         */
static seq_t  a_prev;          /* previous partial quotient a_{k-1} */
static seq_t  prev2 = 0;       /* p_{k-2}                        */
static seq_t  prev1 = 0;       /* p_{k-1}                        */
static bool   initialized = false;     /* flag for one‑time init */

/* Constants */
static const seq_t A0 = (seq_t)14;      /* floor(sqrt(206))             */
static const seq_t D  = (seq_t)206;     /* D in sqrt(D)                  */

seq_t a(idx_t n)
{
    /* One‑time initialization */
    if (!initialized) {
        /* Allocate space for the first term p0 = a0 */
        cache_cap   = 1;
        cache       = seq_realloc(cache, 0, cache_cap);
        cache[0]    = A0;
        cache_size  = 1;

        /* Initial state for generating the first partial quotient a1 */
        m_curr = (seq_t)0;   /* m0   */
        d_curr = (seq_t)1;   /* d0   */
        a_prev = A0;         /* a0   */

        /* Numerator recurrence initial values */
        prev2 = (seq_t)1;    /* p_{-1} */
        prev1 = A0;          /* p_0   */

        initialized = true;
    }

    /* Fast path for already computed values */
    if ((size_t)n < cache_size) {
        return cache[n];
    }

    /* Ensure capacity for the new elements */
    while (cache_size <= (size_t)n) {
        /* Compute the next partial quotient a_next and state update */
        seq_t m_next   = d_curr * a_prev - m_curr;
        seq_t d_next   = (D - m_next * m_next) / d_curr;
        seq_t a_next   = (A0 + m_next) / d_next;

        /* Numerator of the next convergent */
        seq_t p_next   = a_next * prev1 + prev2;

        /* Store the new term */
        if (cache_size + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }
        cache[cache_size] = p_next;
        cache_size++;

        /* Shift recurrence variables for subsequent step */
        prev2 = prev1;
        prev1 = p_next;

        /* Update state for next a_k */
        m_curr = m_next;
        d_curr = d_next;
        a_prev = a_next;
    }

    return cache[n];
}
