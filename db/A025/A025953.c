/*
 * A025953  Expansion of g.f. 1/((1-2*x)*(1-3*x)*(1-9*x)*(1-11*x)).
 * https://oeis.org/A025953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3^(n+1) - 2^(n+1) + 20*a(n-1) - 99*a(n-2),
   a(0) = 1, a(1) = 25.  Indexing follows OEIS (a(0) is first term). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static idx_t      computed_until = (idx_t)-1;   /* No terms computed yet */
    static seq_t     *cache          = NULL;       /* Array of terms */
    static size_t     cache_size     = 0;           /* Allocated array size */
    static seq_t      pow2, pow3;                  /* 2^(computed_until+1), 3^(computed_until+1) */

    /* One‑time initialisation */
    if (computed_until == (idx_t)-1) {
        cache_size = 2;                                   /* a(0) and a(1) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) { abort(); }

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        computed_until = 1;

        pow2 = (seq_t)4;   /* 2^(1+1) */
        pow3 = (seq_t)9;   /* 3^(1+1) */
    }

    /* Return immediately if the desired term is already known */
    if (n <= computed_until) {
        return cache[n];
    }

    /* Grow the cache if required */
    size_t needed = (size_t)n + 1;
    if (cache_size < needed) {
        seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!new_cache) { abort(); }
        cache = new_cache;
        cache_size = needed;
    }

    /* Iteratively compute all missing terms up to n */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        seq_t next_pow2 = pow2 * (seq_t)2;
        seq_t next_pow3 = pow3 * (seq_t)3;

        /* a(i) = 3^(i+1) - 2^(i+1) + 20*a(i-1) - 99*a(i-2) */
        seq_t term = next_pow3 - next_pow2
                   + (seq_t)20 * cache[i - 1]
                   - (seq_t)99 * cache[i - 2];

        cache[i] = term;

        /* Update the power values for the next iteration */
        pow2 = next_pow2;
        pow3 = next_pow3;
    }

    computed_until = n;
    return cache[n];
}
