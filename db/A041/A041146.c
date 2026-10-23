/*
 * A041146  Numerators of continued fraction convergents to sqrt(83).
 * https://oeis.org/A041146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   Numerators of continued fraction convergents to sqrt(83).
 *            Recurrence: p_{-1} = 1, p_0 = 9, p_k = a_k * p_{k-1} + p_{k-2},
 *            where the a_k are the partial quotients of sqrt(83)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t   *cache = NULL;      /* stored numerators           */
static idx_t    cache_size = 0;    /* amount of data in cache     */
static idx_t    cache_cap  = 0;    /* capacity of cache in elems  */

/* Continued‑fraction state for sqrt(83) */
static uint64_t cf_m = 0;          /* m_k                 */
static uint64_t cf_d = 1;          /* d_k                 */
static uint64_t cf_a = 9;          /* a_k (a0 = 9)        */

/* previous two convergent numerators */
static seq_t cf_p_prevprev = (seq_t)1;  /* p_{k-2}           */
static seq_t cf_p_prev     = (seq_t)9;  /* p_{k-1}           */

seq_t a(idx_t n)
{
    /* first call: initialise cache and state */
    if (cache == NULL) {
        cache_cap = 1;
        cache = seq_alloc(cache_cap);
        if (!cache) abort();

        cache[0] = (seq_t)9;      /* a(0) = 9   */
        cache_size = 1;
        /* cf_* variables already set to state after a0   */
    }

    /* generate terms up to index n */
    while (cache_size <= n) {
        /* compute next partial quotient a_next */
        const uint64_t N     = 83;
        const uint64_t a0    = 9;          /* floor(sqrt(83)) */
        uint64_t m_next = cf_d * cf_a - cf_m;
        uint64_t d_next = (N - m_next * m_next) / cf_d;
        uint64_t a_next = (a0 + m_next) / d_next;

        /* compute next convergent numerator */
        seq_t num_next = (seq_t)a_next * cf_p_prev + cf_p_prevprev;

        /* enlarge cache if necessary */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_size++] = num_next;

        /* advance continued‑fraction state */
        cf_m = m_next;
        cf_d = d_next;
        cf_a = a_next;
        cf_p_prevprev = cf_p_prev;
        cf_p_prev = num_next;
    }

    return cache[n];
}
