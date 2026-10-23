/*
 * A033125  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,1.
 * https://oeis.org/A033125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-3) - 7*a(n-4) with initial values a(1)=1,
 *            a(2)=7, a(3)=50, a(4)=351
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* seq_t and idx_t are supplied by the build system */

/* Persistent cache for memoisation */
static seq_t *seq_cache   = NULL;   /* array of cached values, index 0 unused        */
static size_t  cache_size = 0;      /* allocated size (indices 0..size-1)            */
static idx_t   last_computed = 0;   /* highest index currently cached (n>=4 afterwards) */

/* Allocate or grow the cache so it can hold indices up to 'n'. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;      /* space for indices 0..n */
    if (required < 5) required = 5;       /* at least 1..4 must fit */

    if (seq_cache == NULL) {              /* first allocation */
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * required);
        if (!seq_cache) exit(EXIT_FAILURE);
        /* initialise all slots to zero */
        for (size_t i = 0; i < required; ++i) seq_cache[i] = (seq_t)0;
        /* store the initial fixed terms */
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)7;
        seq_cache[3] = (seq_t)50;
        seq_cache[4] = (seq_t)351;
        last_computed = (idx_t)4;
    } else if (required > cache_size) {   /* grow the existing cache */
        seq_t *tmp = (seq_t *)realloc(seq_cache, sizeof(seq_t) * required);
        if (!tmp) exit(EXIT_FAILURE);
        /* initialise new slots to zero */
        for (size_t i = cache_size; i < required; ++i) tmp[i] = (seq_t)0;
        seq_cache = tmp;
    }
    cache_size = required;
}

/* Return the nth term of the sequence (indexing starts at 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for out‑of‑bounds use */

    ensure_cache(n);

    if (n <= last_computed) {
        return seq_cache[(size_t)n];
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* All arithmetic is performed in seq_t; no casting to __int128. */
        seq_t val = (seq_t)7 * seq_cache[(size_t)(i-1)]
                    + seq_cache[(size_t)(i-3)]
                    - (seq_t)7 * seq_cache[(size_t)(i-4)];
        seq_cache[(size_t)i] = val;
    }

    last_computed = n;
    return seq_cache[(size_t)n];
}
