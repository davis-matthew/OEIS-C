/*
 * A042345  Denominators of continued fraction convergents to sqrt(699).
 * https://oeis.org/A042345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   Denominators of continued‑fraction convergents to sqrt(699)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1.  For n>0, a(n) is produced by the standard recurrence for
   continued fractions of sqrt(699).  No explicit linear recurrence is
   required – the values are computed by stepping through the continued
   fraction.  The function memoises previously computed values. */

/* Do not include any headers that are not part of the standard library. */
#include <stddef.h>
#include <stdlib.h>

/* seq_t and idx_t are provided by the build system. */

/* Helper to grow a dynamic array safely. */
static void grow_array(void **p, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 1 : *cap;
        while (new_cap < needed) new_cap *= 2;
        *p = realloc(*p, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Memoisation state: cache and continued‑fraction parameters. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;  /* allocated capacity                     */
    static size_t cache_len = 0;  /* number of computed terms (next free)   */
    static int ready = 0;        /* flag to initialise state once          */

    /* Continued‑fraction state variables. */
    static idx_t m = 0;           /* m_{k} in the algorithm            */
    static idx_t d = 1;           /* d_{k}                               */
    static idx_t a_cf = 0;        /* a_{k}                               */
    static seq_t q_prevprev = 0;  /* q_{k-2}                            */
    static seq_t q_prev = 1;      /* q_{k-1}                            */
    static const idx_t D = 699;   /* radicand of the square root         */
    static const idx_t a0 = 26;   /* floor(sqrt(699)) = 26              */

    /* Initialise static state on first call. */
    if (!ready) {
        ready = 1;
        /* Start with the first convergent: denominator q0 = 1. */
        grow_array((void **)&cache, &cache_cap, 1);
        cache[0] = 1;
        cache_len = 1;          /* next term to produce will be a(1)   */
        m = 0;
        d = 1;
        a_cf = a0;
        q_prevprev = 0;
        q_prev = 1;
    }

    /* Convert requested index to a size_t for array indexing. */
    size_t target = (size_t)n;

    /* Extend the cache up to the requested term. */
    while (cache_len <= target) {
        /* Calculate next partial quotient of the continued fraction. */
        idx_t m_next = d * a_cf - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        /* Compute the next denominator: q = a_next * q_prev + q_prevprev. */
        seq_t q = (seq_t)a_next * q_prev + q_prevprev;

        /* Store the new term, expanding the cache if necessary. */
        grow_array((void **)&cache, &cache_cap, cache_len + 1);
        cache[cache_len] = q;
        cache_len++;

        /* Shift storage for the next iteration. */
        q_prevprev = q_prev;
        q_prev = q;
        m = m_next;
        d = d_next;
        a_cf = a_next;
    }

    return cache[target];
}
