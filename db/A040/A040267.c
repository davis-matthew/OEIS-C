/*
 * A040267  Continued fraction for sqrt(284).
 * https://oeis.org/A040267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=16, thereafter a(n) is the n-th term of the continued *
 *            fraction expansion of sqrt(284) (periodic). The algorithm uses
 *            the * standard recurrence for quadratic irrationals
 * Range:     u8 none, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;

static seq_t a0 = 0;   /* floor(sqrt(284)) */
static seq_t m = 0, d = 1, an = 0;   /* state for the recurrence */

/* Ensure internal cache has room for at least n+1 terms. */
static void ensure_capacity(idx_t n)
{
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            new_cap * sizeof(seq_t));
        if (!new_cache) exit(1); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Initialize on first call. */
    if (cache_len == 0) {
        ensure_capacity(1);
        a0 = floor_sqrt((seq_t)284);
        m   = 0;
        d   = 1;
        an  = a0;
        cache[0] = a0;
        cache_len = 1;
    }

    /* Return cached value if already computed. */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Recurrence for continued fraction of sqrt(284). */
        seq_t m_next = d * an - m;
        seq_t d_next = (seq_t)284 - m_next * m_next;
        d_next /= d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Store new term. */
        ensure_capacity(i);
        cache[(size_t)i] = a_next;

        /* Advance state. */
        m  = m_next;
        d  = d_next;
        an = a_next;
    }

    cache_len = n + 1;
    return cache[(size_t)n];
}
